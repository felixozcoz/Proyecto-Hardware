#include <LPC210x.h>

#include "juego.h"
#include "cola_FIFO.h"
#include "temporizador_drv.h"
#include "botones.h"
#include "tablero.h"
#include "conecta_K_2023.h"
#include "linea_serie_drv.h"
#include "Mensaje_t.h"
#include "celda.h"
#include "GPIO_hal.h"
#include <string.h>
#include <stdlib.h>

// Guarda el tiempo transcurrido
// entre las dos últimas pulsaciones
// static unsigned int	__attribute__((unused)) intervalo; // retirar el "__attribute__((unused))" cuando se utilice intervalo

// Ultima pulsación
// static unsigned int __attribute__((unused)) last_press = 0;


typedef enum {
		LOBBY = 0,	
		ESPERANDO_JUGADA = 1, 
		ESPERANDO_CONFIRMACION = 2,
	} estadoJuego_t;

	typedef enum {
		UNDEFINED = 0,
		VICTORIA = 1,
		RENDICION_BOTON = 2,
		RENDICION_COMANDO = 2,
	} condicionFin_t;


// Información juego
static estadoJuego_t estado;
static uint8_t turno;								// Registra de que jugador es el turno E {0,1}
static condicionFin_t condicion_fin ;
// Almacenamiento y visualización
static TABLERO tablero;																			// Estado del tablero
static uint8_t pantalla[NUM_FILAS + 1][NUM_COLUMNAS + 1]; 	// Visualizacion en memoria
	
// Para contabilizar el timeout de esperando jugada
static uint64_t tic;
static uint64_t tac;
static const uint64_t timeout = 3000000;

// Para guardar provisionalmente la jugada en curso
static uint32_t fila;
static uint32_t columna;
	
// Pin GPIO indicar comando inválido
static GPIO_HAL_PIN_T pin_cmd_no_valido;

// Añadir variables para kpis
	
// Funciones auxiliares de impresión (definida después)
void imprimir_tablero_linea_serie(void);
void imprimir_reglas(void);

// Funcion auxiliar para determinar si una fila-columna son validas y ademas la celda esta libre. (definida despues)
int jugadaValida(TABLERO *tablero, int fila, int columna);



// ***** FUNCIONES ****

// Inicializa el tablero del juego conecta_k
void inicializar_juego(uint8_t tab_input[NUM_FILAS][NUM_COLUMNAS], const GPIO_HAL_PIN_T _pin_cmd_no_valido)
{
		// configurar gpio
	pin_cmd_no_valido = _pin_cmd_no_valido;
	gpio_hal_sentido(pin_cmd_no_valido, 1, GPIO_HAL_PIN_DIR_OUTPUT);
	
		// configurar tablero
	tablero_inicializar(&tablero);
	conecta_K_cargar_tablero(&tablero, tab_input);
	
		// configurar estado de juego
	estado = LOBBY;  						// no se ha iniciado la partida como tal
	condicion_fin = UNDEFINED;  // condición de terminación indefinida
	turno = 0;								// primer turno jugador uno
	
		// mostrar reglas del juego a la espera de un evento que inicie la partida
	imprimir_reglas();
}



/*		NOTA: con respecto a la validez o no validez de los comandos.
			En el guion se especifica que en caso de recibir un comando no valido, se
			escribira por linea serie "Comando erroneo". Pero por lo que entiendo yo en el driver de linea serie
			ya se hace esta comprobacion antes de encolar el evento de lectura con el comando el la variable trama.
			Tal vez habria que quitar esa comprobacion e incluirla en este modulo?
*/


void juego_tratar_evento(const EVENTO_T ID_evento, const uint32_t auxData){
	char msj_info[30];
	
	switch(estado){
		
		case LOBBY:
			
			// Despulsación ó cmd '$NEW!' (nueva partida) --> iniciar partida
			if( ID_evento == ev_DESPULSACION || 
					(ID_evento == ev_RX_SERIE && auxData == ((('N' << 16) | ('E' << 8) | 'W')))) {
					// imprimir tablero
				imprimir_tablero_linea_serie();
					// indicar primer turno a jugador 1
				snprintf(msj_info, sizeof(msj_info), "Turno de jugador %u\n", turno);
				linea_serie_drv_enviar_array( msj_info );
					// actualiza estado
				estado = ESPERANDO_JUGADA;
			}
			
			break;
		
		case ESPERANDO_JUGADA:
				// indicar turno			
			snprintf(msj_info, sizeof(msj_info), "Turno de jugador %u\n", turno);
			linea_serie_drv_enviar_array( msj_info );
				
			if ( ID_evento == ev_DESPULSACION && auxData == BOTON_2) {
				// kpis y lo que haya que mostrar al terminar la partida
				condicion_fin = RENDICION_BOTON;
			} 
			else if(ID_evento == ev_RX_SERIE && auxData == ((('E' << 16) | ('N' << 8) | 'D'))){
				// kpis y lo que haya que mostrar al terminar la partida
				condicion_fin = RENDICION_COMANDO;
			} 
			else if(ID_evento == ev_RX_SERIE && auxData == ((('N' << 16) | ('E' << 8) | 'W'))){
				// Si el comando es NEW se ignora
				// Ahora por descarte si llega un comando sera una jugada. (Ver NOTA)
			} 
			else if(ID_evento == ev_RX_SERIE){
				fila = (auxData >> 16) & 0xFF;
				columna = auxData & 0xFF;
				// Para comprobar que una jugada es valida
				// - Tablero_fila_valida && tablero_columna_valida (Esto lo hace tambien el driver linea_serie, ver NOTA)
				// - Comprobar celda vacia.
				if (jugadaValida(&tablero, fila, columna)){
					// escribir caracter especial en la casilla selecionada
					imprimir_tablero_linea_serie();
					tic = temporizador_drv_leer();
					estado = ESPERANDO_CONFIRMACION;
				} 
				else{
						// comando erróneo, se ignora e indica por gpio
					linea_serie_drv_enviar_array("Comando erroneo\n");
					gpio_hal_escribir(pin_cmd_no_valido, 1, 1); // gpio29 on
				}
			} 
				
			// Añadir aqui comprobación de comando no valido.				???
		break;
				
				
			
		case ESPERANDO_CONFIRMACION:
			if( ID_evento == ev_LATIDO) {
				tac = temporizador_drv_leer();
				if( tac - tic >= timeout) {
					// Eliminar caracter especial del tablero
					// Colocar ficha teniendo en cuenta la variable turno para saber de que jugador es
					tablero_insertar_color(&tablero, fila, columna, turno+1);
					imprimir_tablero_linea_serie();
					turno = !turno;																// Cambio de turno
					// Comprobar alineamiento y proceder
				}
			}
			// Añadir comprobacion de despulsacion de boton 2 y comando end para rendicion		???
			// Añadir aqui comprobación de comando no valido.
		break;
	}
	

	
//	unsigned int now = clock_get_us(); // en ms
//	intervalo = (now - last_press);
//	last_press = now;
//	
//	if(auxData == BOTON_1)	// EINT1
//		cuenta ++;
//	else	// EINT2
//		cuenta--;
//	
//	FIFO_encolar(ev_VISUALIZAR_CUENTA, cuenta); // visualizar cuenta en GPIO
	
	// recibido ev_RX_SERIE
//	if (ID_evento == ev_RX_SERIE){
//			conecta_K_visualizar_tablero(&tablero, pantalla);
//			imprimir_tablero_linea_serie();
//	}
}


// Función para comprobar si una casilla está ocupada
int jugadaValida(TABLERO *tablero, int fila, int columna) {
		size_t i;
	
    if(!tablero_fila_valida(fila) || !tablero_columna_valida(columna)){
			return 0;
		}
		
		 // Si se encuentra la columna, la casilla está ocupada
    for (i = 0; i < NUM_FILAS; i++) 
			if (tablero->columnas[fila][i] == columna)  return i;

    // Si no se encuentra la columna, la casilla no está ocupada
    return 0;
}



void imprimir_reglas(void)
{
	linea_serie_drv_enviar_array("Aqui van las reglas\n");
}


// Determina si el contenido extraído de la línea serie,
// bien sea comando o jugada, es válido. En caso de que no serlo
// muestra un mensaje indicándolo por línea seríe así como gpio
void comprobar_contenido_linea_serie(const uint32_t auxData)
{
		
	
	
}



void imprimir_tablero_linea_serie(void)
{
	Mensaje_t msg;
	
	int offset = 0;
	size_t i,j;
	sprintf(msg + offset, "-|");
	offset+=2;
		// imprimir fila indicadora de columnas
  for (j = 1; j <= NUM_COLUMNAS; j++) {
     offset += sprintf(msg + offset, "%d|", j);
  }
	offset += sprintf(msg + offset, "\n");
	
  for (i = 1; i <= NUM_FILAS; i++) {
     offset += sprintf(msg + offset, "%d|", i);
     for (j = 0; j < NUM_COLUMNAS; j++) {
							// leer valor de tablero
						CELDA celda = tablero_leer_celda(&tablero, i-1, j);
						celda = celda_color(celda);
							// añadir caracteres según valor
            if (celda == 0)	
                offset += sprintf(msg + offset, " |");
            else if (celda == 1)
                offset += sprintf(msg + offset, "B|");
            else if (celda == 2)
                offset += sprintf(msg + offset, "N|");
           
        }
        offset += sprintf(msg + offset, "\n");
    }
		
		linea_serie_drv_enviar_array(msg);
}

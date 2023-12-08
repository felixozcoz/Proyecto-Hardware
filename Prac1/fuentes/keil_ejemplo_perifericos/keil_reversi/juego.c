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

// Contabiliza las pulsaciones de los botone
// pulsar EINT1 incrementa en una unidad y
// pulsar EINT2 decrementa en una unidad
// static int __attribute__((unused)) cuenta;

// Guarda el tiempo transcurrido
// entre las dos �ltimas pulsaciones
// static unsigned int	__attribute__((unused)) intervalo; // retirar el "__attribute__((unused))" cuando se utilice intervalo

// Ultima pulsaci�n
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


// Informaci�n juego
static estadoJuego_t estado;
static condicionFin_t condicion;
static TABLERO tablero;																															// Estado del tablero
static uint8_t pantalla[NUM_FILAS + 1][NUM_COLUMNAS + 1]; 		// Visualizacion en memoria
static uint8_t turno;																																			// Registra de que jugador es el turno E {0,1}
	
// Para contabilizar el timeout de esparando jugada
static uint64_t tic;
static uint64_t tac;
static const uint64_t timeout = 3000000;

// Par guardar probisionalmente la jugada en curso
static uint32_t fila;
static uint32_t columna;

// A�adir variables para kpis
	

// Funci�n auxiliar (definida despues)
void imprimir_tablero_linea_serie(void);

// Funcion auxiliar para determinar si una fila-columna son validas y ademas la celda esta libre. (definida despues)
int jugadaValida(TABLERO *tablero, int fila, int columna);

// Inicializa el tablero del juego conecta_k
void inicializar_juego(uint8_t tab_input[NUM_FILAS][NUM_COLUMNAS]){
	tablero_inicializar(&tablero);
	conecta_K_cargar_tablero(&tablero, tab_input);
	turno = 0;				// ??
}



/*		NOTA: con respecto a la validez o no validez de los comandos.
			En el guion se especifica que en caso de recibir un comando no valido, se
			escribira por linea serie "Comando erroneo". Pero por lo que entiendo yo en el driver de linea serie
			ya se hace esta comprobacion antes de encolar el evento de lectura con el comando el la variable trama.
			Tal vez habria que quitar esa comprobacion e incluirla en este modulo?
*/


void juego_tratar_evento(const EVENTO_T ID_evento, const uint32_t auxData){
	switch(estado){
		case LOBBY:
			// Si evento despulsacion o evento lectura linea serie y comando NEW
			if(ID_evento == ev_DESPULSACION || (ID_evento == ev_RX_SERIE && auxData == ((('N' << 16) | ('E' << 8) | 'W')))) {
				// Hacer aqui lo que sea necesario para inicializar el juego
				imprimir_tablero_linea_serie();
				estado = ESPERANDO_JUGADA;
			}
			
			// A�adir aqui comprobaci�n de comando no valido.			???
		break;
		
			
		
		case ESPERANDO_JUGADA:
			if ( ID_evento == ev_DESPULSACION && auxData == 2) {
				// kpis y lo que haya que mostrar al terminar la partida
				condicion = RENDICION_BOTON;
			} else if(ID_evento == ev_RX_SERIE && auxData == ((('E' << 16) | ('N' << 8) | 'D'))){
				// kpis y lo que haya que mostrar al terminar la partida
				condicion = RENDICION_COMANDO;
			} else if(ID_evento == ev_RX_SERIE && auxData == ((('N' << 16) | ('E' << 8) | 'W'))){
				// Si el comando es NEW se ignora
				// Ahora por descarte si llega un comando sera una jugada. (Ver NOTA)
			} else if(ID_evento == ev_RX_SERIE){
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
				} else{
					// Escribir mensaje "Fila-Columna no valida"
					// Encender GPIO29
				}
			} 
				
			// A�adir aqui comprobaci�n de comando no valido.				???
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
			// A�adir comprobacion de despulsacion de boton 2 y comando end para rendicion		???
			// A�adir aqui comprobaci�n de comando no valido.
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

// Funci�n para comprobar si una casilla est� ocupada
int jugadaValida(TABLERO *tablero, int fila, int columna) {
    if(!tablero_fila_valida(fila) || !tablero_columna_valida(columna)){
			return 0;
		}
		
    for (int i = 0; i < NUM_FILAS; i++) {
        if (tablero->columnas[fila][i] == columna) {
            // Si se encuentra la columna, la casilla est� ocupada
            return 1;
        }
    }

    // Si no se encuentra la columna, la casilla no est� ocupada
    return 0;
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
							// a�adir caracteres seg�n valor
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

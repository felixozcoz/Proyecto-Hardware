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
#include "tramas.h"
#include "alarmas.h"

typedef enum {
		LOBBY = 0,	
		ESPERANDO_JUGADA = 1, 
		ESPERANDO_CONFIRMACION = 2,
}  
estadoJuego_t;

typedef enum {
		UNDEFINED = 0,
		VICTORIA = 1,
		RENDICION_BOTON = 2,
		RENDICION_COMANDO = 3,
}  
condicionFin_t;

typedef enum {
		JUGADOR1 = 0,
		JUGADOR2 = 1,
}  
turno_t;

// Información juego
static estadoJuego_t estado;
static turno_t turno;															
static condicionFin_t condicion_fin ;

// Almacenamiento y visualización
static TABLERO tablero;																			// Estado del tablero
static uint8_t pantalla[NUM_FILAS + 1][NUM_COLUMNAS + 1]; 	// Visualizacion en memoria
	
// Para contabilizar el timeout de esperando confirmación
static uint64_t ticEsperaConfirmacion;
static uint64_t tacEsperaConfirmacion;
static const uint64_t timeout = 3000000;

// Para calcular: tiempo total y media esperando jugada
static uint64_t ticEsperandoJugada;
static uint64_t tacEsperandoJugada;
static uint64_t tTotalEsperandoJugada;
static uint64_t nVecesEsperandoJugada;

// Para calcular: tiempo total y media en ejecutar conecta_K_hay_linea
static uint64_t ticHayLinea;
static uint64_t tacHayLinea;
static uint64_t tTotalHayLinea;
static uint64_t nVecesHayLinea;

// Para guardar provisionalmente la jugada en curso
static int fila;
static int columna;
	
// Pin GPIO indicar comando inválido
static GPIO_HAL_PIN_T pin_cmd_no_valido;

// Añadir variables para kpis
	
// Funciones auxiliares de impresión (definida después)
void imprimir_tablero_linea_serie(void);
void imprimir_reglas(void);
void imprimir_stats_finalizacion(void);
void imprimir_vista_inicial_nueva_partida(void);

// Funcion auxiliar para determinar si una fila-columna son validas y ademas la celda esta libre. (definida después)
bool jugada_valida(TABLERO *tablero, const int fila, const int columna);

// Dada la trama 'inputTrama' determina si es válida y
// o no y actúa en consecuencia
bool comprobar_trama(const uint32_t inputTrama);

// Función auxiliar que determina si 'inputTrama' es una trama jugada
bool esTramaJugada(const uint32_t inputTrama);

// Función auxiliar
void cambiarTurno(void);


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
	
	tTotalEsperandoJugada = 0;
	nVecesEsperandoJugada = 0;
	
	tTotalHayLinea = 0;
	nVecesHayLinea = 0;
	
	ticEsperaConfirmacion = 0;
	tacEsperaConfirmacion = 0;
	
		// configurar estado de juego
	estado = LOBBY;  						
	condicion_fin = UNDEFINED; 
	turno = JUGADOR1;
	fila = -1;
	columna = -1;
	
		// mostrar reglas del juego a la espera de un evento que inicie la partida
	imprimir_reglas();
}


void juego_tratar_evento(const EVENTO_T ID_evento, const uint32_t auxData){
	char msj_info[30]; // almacena mensajes informativos
	
	switch(estado){
		
		case LOBBY:
			
				// Despulsación ó cmd '$NEW!' (nueva partida) --> iniciar partida
			if( ID_evento == ev_DESPULSACION || (ID_evento == ev_RX_SERIE && auxData == trama_NEW)) {
					imprimir_vista_inicial_nueva_partida();
						// actualización de estado
					estado = ESPERANDO_JUGADA;
					nVecesEsperandoJugada++;
					ticEsperandoJugada = clock_get_us();
			}
			break;
		
		case ESPERANDO_JUGADA:

			if ( ID_evento == ev_DESPULSACION && auxData == BOTON_2) {
				// kpis y lo que haya que mostrar al terminar la partida
				condicion_fin = RENDICION_BOTON;
				imprimir_stats_finalizacion(); // mostrar stats
				
				tacEsperandoJugada = clock_get_us();
				tTotalEsperandoJugada += tacEsperandoJugada - ticEsperandoJugada;
				
			} 
				// mensaje recibido por línea serie && tratar trama
			else if (ID_evento == ev_RX_SERIE && comprobar_trama(auxData)){
					// indicar turno			
				snprintf(msj_info, sizeof(msj_info), "Turno de jugador %u\n", turno + 1);
				linea_serie_drv_enviar_array( msj_info );
			}
			
			break;
			
		case ESPERANDO_CONFIRMACION:
			
			if( ID_evento == ev_DESPULSACION && auxData == BOTON_1)
			{
				snprintf(msj_info, sizeof(msj_info), "Movimiento cancelado\n");
				linea_serie_drv_enviar_array( msj_info );
					// cancelar movimiento y sigue turno de mismo jugador
				fila = -1; columna = -1; // eliminar símbolo de casila pendiente de confirmar
				imprimir_tablero_linea_serie();
				estado = ESPERANDO_JUGADA;
			}
			else if(ID_evento == ev_LATIDO){
				if( (tacEsperaConfirmacion - ticEsperaConfirmacion) >= timeout ) 
				{
						// actualizar tablero
					tablero_insertar_color(&tablero, fila, columna, turno + 1);
					fila = -1; columna = -1; // eliminar símbolo de casila pendiente de confirmar
					imprimir_tablero_linea_serie();
						// actualizar estado
					turno = !turno;
					estado = ESPERANDO_JUGADA;
				}
				else{
					tacEsperaConfirmacion = clock_get_us();
				}
			}
		break;
	}
}


// jugada_valida
//
// Devuelve 1 si la celda (fila, columna) está ocupada.
// En cualquier otro caso devuelve 0
bool jugada_valida(TABLERO *tablero, const int _fila, const int _columna) {
			
			// si celda ocupada jugada inválida
		if( ! celda_vacia( tablero_leer_celda(tablero, _fila-1, _columna-1) ) )
			return 0;
		
			// guardar posición provisional de ficha pendiente de confirmar
    fila = _fila-1; 
		columna = _columna-1; 
		
		return 1;
}

// es_trama_jugada_valida
//
// Parámetros:
// 	trama: contiene codificada en ASCII una trama
//
// Devuelve 1 si 'trama' es una trama válida de 
// acuerdo al formato de trama de tipo jugada
// acordado e indicado por la siguiente exp. reg. "[0-9]-[0-9]".
// En cualquier otro caso devuelve 0.
bool es_trama_jugada_valida(uint32_t trama) {
    // Extrae los caracteres de la trama codificada en ASCII
    char char1 = (trama >> 16) & 0xFF;
    char char2 = (trama >> 8) & 0xFF;
    char char3 = trama & 0xFF;

    // Verifica que char1 y char3 sean dígitos y char2 sea '-'
    if ((char1 >= '1' && char1 <= (NUM_FILAS + '0') ) &&
        (char2 == '-') &&
        (char3 >= '1' && char3 <= (NUM_COLUMNAS + '0') )) {
        return jugada_valida(&tablero, char1 - '0', char3 - '0');
		}	
				
    return 0; // La trama de jugada no es válida
}

// TODO: revisar 
// comprobar_trama
//
// Parámetros:
//	inputTrama: trama codificada en un uint32_t
//
// Si 'inputTrama' corresponde a una trama del 
// dominio del juego, esta es encolada con su evento
// correspondiente. En cualquier otro caso, se
// muestra un mensaje por linea serie así 
// como por gpio asociado al juego
bool comprobar_trama(const uint32_t inputTrama)
{					
	switch(inputTrama){
	
		case trama_NEW:
				// nueva partida
			if(estado == LOBBY)
				FIFO_encolar(ev_NUEVA_PARTIDA, inputTrama); //  notificar para iniciar de alarma usuario ausente
			break;
	
		case trama_END:			
				// terminar partida
			if( estado != LOBBY){
				// kpis
				condicion_fin = RENDICION_COMANDO;
				imprimir_stats_finalizacion(); // mostrar stats
				FIFO_encolar(ev_TERMINAR_PARTIDA, inputTrama); //  notificar para finalizar de alarma usuario ausente
			}
			break;
		
		default:
			// por defecto: nueva jugada
			if( (estado == ESPERANDO_JUGADA) && esTramaJugada(inputTrama) )
			{
				if( es_trama_jugada_valida(inputTrama) ) 
				{
						imprimir_tablero_linea_serie(); 				// la casilla se añade automáticamente desde dentro de la función
						ticEsperaConfirmacion = clock_get_us(); //temporizador_drv_leer(); 
						estado = ESPERANDO_CONFIRMACION; 				// actualiza estado
					
						FIFO_encolar(ev_NUEVA_JUGADA, inputTrama); //  notificar para reset de alarma usuario ausente
				}
				else {	
						linea_serie_drv_enviar_array("Jugada no valida\n");
						gpio_hal_escribir(pin_cmd_no_valido, 1, 1);	
				}
			}
				// cualquier otro comando fuera del dominio del juego
			else{
					linea_serie_drv_enviar_array("Comando erroneo\n");
					gpio_hal_escribir(pin_cmd_no_valido, 1, 1);	
					return 0; // indica comando erróneo
			}
			
		} // switch
		return 1; // comando dentro del dominio del juego
}

// imprimir_reglas
//
// Envía por línea serie las reglas del 
// juego que eventualmente serán mostradas
// por línea serie
void imprimir_reglas(void)
{
	linea_serie_drv_enviar_array("Aqui van las reglas\n");
}


// TODO
//
// imprimir_stats_finalizacion
//
// Muestra por línea serie:
// 	- Causa finalización
//	- Tiempo total de uso de procesador en esta partida (sin power-down)
//	- Total y media de tiempo de computo de conecta_K_hay_linea
//	- Total y media ed tiempo que al humano le cuesta pensar jugada
//	- Total de eventos encolados en esta partida
//	- Historiograma por tipo de evento
void imprimir_stats_finalizacion(void){
		Mensaje_t msg_info;
		
		linea_serie_drv_enviar_array("Las stats...\n");
	
		switch(condicion_fin){
			
			case VICTORIA:
				sprintf(msg_info, "EL JUGADOR %u HA GANADO!!! \n", turno+1);
				linea_serie_drv_enviar_array(msg_info);
				break;
			
			case RENDICION_BOTON:
				sprintf(msg_info, "El jugador %u se ha rendido tras presionar el boton de rendicion\n", turno+1);
				linea_serie_drv_enviar_array(msg_info);
				break;
			
			case RENDICION_COMANDO:
				sprintf(msg_info, "El jugador %u se ha rendido tras introducir comando 'END'\n", turno+1);
				linea_serie_drv_enviar_array(msg_info);
				break;
			
			default:
				// no ha podido llegar hasta aquí
				break;
		}
		estado = LOBBY; 
		// imprimir leyenda para jugar de nuevo()
		// reiniciar juego
}
	

// esTramaJugada
//
// Devuelve 1 si 'inputTrama' es una trama
// de tipo jugada
bool esTramaJugada(const uint32_t inputTrama){
	return ( inputTrama & 0x00FF00) == trama_JUGADA;
}





// imprimir_tablero_linea_serie
//
// Envía por línea serie el estado actual 
// del tablero que eventualmente es 
// impreso en la línea serie
void imprimir_tablero_linea_serie(void)
{
	Mensaje_t msg;
	CELDA celda;
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
							// añadir casilla pendiente de confirmación
						if( fila+1 == i && columna == j){
							offset += sprintf(msg + offset, "*|");
							continue;
						}
							// leer valor de tablero
						celda = tablero_leer_celda(&tablero, i-1, j);
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
	offset += sprintf(msg + offset, "\n"); 
	linea_serie_drv_enviar_array(msg);
}

// imprimir_vista_inicial_nueva_partida
//
// Envía por linea serie un mensaje informativo
// del comienzo de la partida, el tablero inicial
// y un mensaje indicando qué jugador empieza
void imprimir_vista_inicial_nueva_partida(void){
		char msg_info[MAX_MENSAJE_LENGTH];
			// mensaje inicio
		linea_serie_drv_enviar_array("--- COMIENZA LA PARTIDA ---\n\n");
		imprimir_tablero_linea_serie(); 
	
			// indicar turno (inicia siempre el jugador 1)			
		snprintf(msg_info, sizeof(msg_info), "Empieza moviendo jugador %u\n\n", turno+1);
		linea_serie_drv_enviar_array( msg_info );
}	


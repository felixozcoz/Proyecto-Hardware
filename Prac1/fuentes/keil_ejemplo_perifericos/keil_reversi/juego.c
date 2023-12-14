#include <LPC210x.h>

#include "juego.h"
#include "cola_FIFO.h"
#include "temporizador_drv.h"
#include "tablero.h"
#include "botones.h"
#include "conecta_K_2023.h"
#include "linea_serie_drv.h"
#include "Mensaje_t.h"
#include "GPIO_hal.h"
#include <string.h>
#include "tramas.h"

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

// Informaci�n juego
static estadoJuego_t estado;
static turno_t turno = JUGADOR1;															
static condicionFin_t condicion_fin ;

// Almacenamiento y visualizaci�n
static TABLERO tablero;
	
// Para contabilizar el timeout de esperando confirmaci�n
static uint64_t ticEsperaConfirmacion;
static uint64_t tacEsperaConfirmacion;
const uint64_t timeout = 3000000;

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

// Para guardar provisionalmente la jugada en curso a espera de confirmaci�n
// Si fila_jugada_por_confirmar = columna_jugada_por_confirmar = -1, el usuario a�n no ha introducido una jugada
static int fila_jugada_por_confirmar;
static int columna_jugada_por_confirmar;
	
// Pin GPIO indicar comando inv�lido
static GPIO_HAL_PIN_T pin_cmd_no_valido;
	
// Funciones auxiliares de impresi�n (definida despu�s)
void imprimir_tablero_linea_serie(void);
void imprimir_reglas(void);
void imprimir_stats(void);
void imprimir_vista_inicial_nueva_partida(void);
void imprimir_leyenda_juego(void);

// Funcion auxiliar para determinar si una fila-columna son validas y ademas la celda esta libre. (definida despu�s)
bool jugada_valida(TABLERO *tablero, const int fila, const int columna);

// Dada la trama 'inputTrama' determina si es v�lida y
// o no y act�a en consecuencia
void comprobar_trama(const uint32_t inputTrama);

// Funci�n auxiliar que determina si 'inputTrama' es una trama jugada
bool esTramaJugada(const uint32_t inputTrama);

void inicializarVariables(void);

// Configura una nueva partida e indica c�mo actuar en esta situaci�n
void finalizar_partida(void);

// ***** FUNCIONES ****

// Inicializa el tablero del juego conecta_k
void inicializar_juego(uint8_t tab_input[NUM_FILAS][NUM_COLUMNAS], const GPIO_HAL_PIN_T _pin_cmd_no_valido)
{ 
	
	inicializarVariables();
		// configurar gpio
	pin_cmd_no_valido = _pin_cmd_no_valido;
	gpio_hal_sentido(pin_cmd_no_valido, 1, GPIO_HAL_PIN_DIR_OUTPUT);
	
	tablero_inicializar(&tablero);
	conecta_K_cargar_tablero(&tablero, tab_input);
}

void inicializarVariables(void){
		// inicializar tablero vac�o
	tablero_inicializar(&tablero);
	
	tTotalEsperandoJugada = 0;
	nVecesEsperandoJugada = 0;
	
	tTotalHayLinea = 0;
	nVecesHayLinea = 0;
	
	ticEsperaConfirmacion = 0;
	tacEsperaConfirmacion = 0;
	
		// configurar estado de juego
	estado = LOBBY;  						
	condicion_fin = UNDEFINED; 
		// indicar que no hay jugada pendiente de confirmaci�n
	fila_jugada_por_confirmar = -1;
	columna_jugada_por_confirmar = -1;
	
		// mostrar reglas del juego a la espera de un evento que inicie la partida
	imprimir_reglas();
}


void juego_tratar_evento(const EVENTO_T ID_evento, const uint32_t auxData){
	Mensaje_t msj_info; // almacena mensajes informativos
	uint8_t hayLinea;
	
	switch(estado){
		
		case LOBBY:
			
				// Despulsaci�n � cmd '$NEW!' (nueva partida) --> iniciar partida
			if( ID_evento == ev_DESPULSACION || (ID_evento == ev_RX_SERIE && auxData == trama_NEW)) {
					imprimir_vista_inicial_nueva_partida();
				
						// actualizaci�n de estado
					estado = ESPERANDO_JUGADA;
				
						// actualizar estad�sticas de juego
					nVecesEsperandoJugada++;
					ticEsperandoJugada = clock_get_us();
			}
			break;
		
		case ESPERANDO_JUGADA:

			if ( ID_evento == ev_DESPULSACION && auxData == BOTON_2) 
			{
				condicion_fin = RENDICION_BOTON;
				imprimir_stats();
				finalizar_partida();
			} 
			else if (ID_evento == ev_RX_SERIE) 	// mensaje recibido por l�nea serie && tratar trama
						comprobar_trama(auxData);
			
			break;
			
		case ESPERANDO_CONFIRMACION:
			
			if( ID_evento == ev_DESPULSACION && auxData == BOTON_1) {
				linea_serie_drv_enviar_array( "Movimiento cancelado\n" );
				
					// cancelar movimiento y sigue turno de mismo jugador
				fila_jugada_por_confirmar = -1;  // eliminar s�mbolo de posici�n pendiente de confirmar
				columna_jugada_por_confirmar = -1; 
				imprimir_tablero_linea_serie();
				estado = ESPERANDO_JUGADA;	
			}
			else if(ID_evento == ev_LATIDO) 
			{
				tacEsperaConfirmacion = clock_get_us();
				if( (tacEsperaConfirmacion - ticEsperaConfirmacion) >= timeout ) 
				{
						// actualizar tablero
					tablero_insertar_color(&tablero, fila_jugada_por_confirmar, columna_jugada_por_confirmar, turno + 1);
							
					ticHayLinea = clock_get_us();
					
						// buscando soluci�n
					hayLinea = conecta_K_hay_linea_c_c(&tablero, fila_jugada_por_confirmar, columna_jugada_por_confirmar, turno + 1);
					
					tacHayLinea = clock_get_us();
					tTotalHayLinea += tacHayLinea - ticHayLinea;
					nVecesHayLinea ++;
					
						// jugada confirmada, restaura fila y columna de jugada pendiente
					fila_jugada_por_confirmar = -1; 
					columna_jugada_por_confirmar = -1; 
					
						// cambiar turno
					turno = !turno;

						// actualizar estado
					if(hayLinea == TRUE)
					{
						imprimir_tablero_linea_serie();
						condicion_fin = VICTORIA;
						imprimir_stats(); 
						finalizar_partida();
	
					} else{
							estado = ESPERANDO_JUGADA;
							// indicar turno			
							snprintf(msj_info, sizeof(msj_info), "Turno de jugador %u\n", turno + 1);
							linea_serie_drv_enviar_array( msj_info );
							imprimir_tablero_linea_serie();
					}
				}
			}
		break;
	}
}

// jugada_valida
//
// Devuelve 1 si la celda (fila, columna) est� ocupada.
// En cualquier otro caso devuelve 0
bool jugada_valida(TABLERO *tablero, const int _fila, const int _columna) {
			
			// si celda ocupada jugada inv�lida
		if( ! celda_vacia( tablero_leer_celda(tablero, _fila-1, _columna-1) ) )
			return 0;
		
			// guardar posici�n provisional de ficha pendiente de confirmar
    fila_jugada_por_confirmar = _fila-1; 
		columna_jugada_por_confirmar = _columna-1; 
		
		return 1;
}

// es_trama_jugada_valida
//
// Par�metros:
// 	trama: contiene codificada en ASCII una trama
//
// Devuelve 1 si 'trama' es una trama v�lida de 
// acuerdo al formato de trama de tipo jugada
// acordado e indicado por la siguiente exp. reg. "[0-9]-[0-9]".
// En cualquier otro caso devuelve 0.
bool es_trama_jugada_valida(uint32_t trama) {
    // Extrae los caracteres de la trama codificada en ASCII
    char char1 = (trama >> 16) & 0xFF;
    char char2 = (trama >> 8) & 0xFF;
    char char3 = trama & 0xFF;

    // Verifica que char1 y char3 sean d�gitos y char2 sea '-'
    if ((char1 >= '1' && char1 <= (NUM_FILAS + '0') ) &&
        (char2 == '-') &&
        (char3 >= '1' && char3 <= (NUM_COLUMNAS + '0') )) {
        return jugada_valida(&tablero, char1 - '0', char3 - '0');
		}	
				
    return 0; // La trama de jugada no es v�lida
}

// TODO: revisar 
// comprobar_trama
//
// Par�metros:
//	inputTrama: trama codificada en un uint32_t
//
// Si 'inputTrama' corresponde a una trama del 
// dominio del juego, esta es encolada con su evento
// correspondiente. En cualquier otro caso, se
// muestra un mensaje por linea serie as� 
// como por gpio asociado al juego
void comprobar_trama(const uint32_t inputTrama)
{					
	switch(inputTrama){
	
		case trama_END:			
				// terminar partida
			if( estado != LOBBY){
				if ( estado == ESPERANDO_JUGADA) {
						tacEsperandoJugada = clock_get_us();
						tTotalEsperandoJugada += tacEsperandoJugada - ticEsperandoJugada;
				}
				condicion_fin = RENDICION_COMANDO;
				imprimir_stats();
				finalizar_partida();
			}
			break;
		
		default:
				// comprobar si trama es una jugada
			if( (estado == ESPERANDO_JUGADA) && esTramaJugada(inputTrama) )
			{
				if( es_trama_jugada_valida(inputTrama) ) 
				{
						imprimir_tablero_linea_serie(); 				
						ticEsperaConfirmacion = clock_get_us(); 
						estado = ESPERANDO_CONFIRMACION; 			
				}
				else {	
						linea_serie_drv_enviar_array("Jugada no valida\n");
						gpio_hal_escribir(pin_cmd_no_valido, 1, 1);		// indicar por gpio
				}
			}
				// cualquier otro comando fuera del dominio del juego
			else{
					linea_serie_drv_enviar_array("Comando erroneo\n");
					gpio_hal_escribir(pin_cmd_no_valido, 1, 1); // indicar por gpio
			}
		} 
}

// finalizar_partida
//
// Configura una nueva partida y muestra
// por l�nea serie c�mo puede actuar 
// el usuario en esta situaci�n
void finalizar_partida(void){	
		estado = LOBBY;
		inicializarVariables();		
			// imprimir leyenda de juego
		linea_serie_drv_enviar_array("Para iniciar una nueva partida escriba '$NEW!' o pulse uno de los botones\n");
}


// imprimir_reglas
//
// Env�a por l�nea serie las reglas del 
// juego que eventualmente ser�n mostradas
// por l�nea serie
void imprimir_reglas(void)
{	
	linea_serie_drv_enviar_array(
										 "Conecta K es un juego donde dos jugadores que\n"
                     "compiten para conectar K fichas\n"
                     "en l�nea ya sea horizontal, vertical o diagonal.\n\n"
	
										 "Cada jugador tiene un movimiento en su turno\n"
										 "en el que debe colocar una ficha en\n"
										 "uno de los huecos del tablero\n"
										 "Una vez realizada una jugada dispones de 3s para cancelarla\n"
										 "sino, se hace efectiva y se muestra en el tablero\n"
										 "Los movimientos se realizan enviando comandos\n"
                     "a trav�s de la linea serie o presionando los botones 1 y 2.\n"
	
										 "La partida finaliza si ganas o el otro jugador se rinde\n\n"
	
										 "Comandos para jugar:\n"
										 "'$NEW!' inicia una nueva partida\n"
										 "'$END!' termina la partida en curso por rendici�n\n"
										 "'$X-Y!' indica una jugada, donde X es la fila e Y la columna\n"
										 "Botones:\n"
										 "Boton 1 (pin gpio 14) permite cancelar una jugada\n"
										 "valida antes de ser confirmada\n"
										 "Boton 2 (pin gpio 15) termina la partida en curso por rendici�n\n\n"

										 "Para iniciar la partida escriba '$NEW!' o pulse uno de los botones\n"
										 "�Divi�rtete jugando!\n\n");
}


// imprimir_stats
//
// Muestra por l�nea serie:
// 	- Causa finalizaci�n
//	- Tiempo total de uso de procesador en esta partida (sin power-down)
//	- Total y media de tiempo de computo de conecta_K_hay_linea
//	- Total y media ed tiempo que al humano le cuesta pensar jugada
//	- Total de eventos encolados en esta partida
//	- Historiograma por tipo de evento
void imprimir_stats(void){
		Mensaje_t msg_info;
	
	  linea_serie_drv_enviar_array("\nEstadisticas de partida: \n\n");
		sprintf(msg_info, "Tiempo total de computo de conecta_K_hay_linea: %" PRIu64 "\n", tTotalHayLinea);
		linea_serie_drv_enviar_array(msg_info);
		sprintf(msg_info, "Tiempo medio computo de conecta_K_hay_linea: %" PRIu64 "\n", tTotalHayLinea/ nVecesHayLinea);
		linea_serie_drv_enviar_array(msg_info);
		sprintf(msg_info, "Tiempo total esperando al jugador: %" PRIu64 "\n", tTotalEsperandoJugada);
		linea_serie_drv_enviar_array(msg_info);
		sprintf(msg_info, "Tiempo medio esperando al jugador: %" PRIu64 "\n", tTotalEsperandoJugada/ nVecesEsperandoJugada);
		linea_serie_drv_enviar_array(msg_info);
	// TODO historiograma
		
		switch(condicion_fin){
			
			case VICTORIA:
				sprintf(msg_info, "EL JUGADOR %u HA GANADO!!! \n", (!turno)+1);
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
				// no ha podido llegar hasta aqu�
				break;
		}
}
	

// esTramaJugada
//
// Devuelve 1 si 'inputTrama' es una trama
// de tipo jugada
__inline bool esTramaJugada(const uint32_t inputTrama){
	return ( inputTrama & 0x00FF00) == trama_JUGADA;
}

// imprimir_tablero_linea_serie
//
// Env�a por l�nea serie el estado actual 
// del tablero que eventualmente es 
// impreso en la l�nea serie
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
							// a�adir casilla pendiente de confirmaci�n
						if( fila_jugada_por_confirmar+1 == i && columna_jugada_por_confirmar == j){
							offset += sprintf(msg + offset, "*|");
							continue;
						}
							// leer valor de tablero
						celda = tablero_leer_celda(&tablero, i-1, j);
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
	offset += sprintf(msg + offset, "\n"); 
	linea_serie_drv_enviar_array(msg);
}

// imprimir_vista_inicial_nueva_partida
//
// Env�a por linea serie un mensaje informativo
// del comienzo de la partida, el tablero inicial
// y un mensaje indicando qu� jugador empieza
void imprimir_vista_inicial_nueva_partida(void){
		Mensaje_t msg_info;
			// mensaje inicio
		linea_serie_drv_enviar_array("--- COMIENZA LA PARTIDA ---\n\n");
		imprimir_tablero_linea_serie(); 
	
			// indicar turno (inicia siempre el jugador 1)			
		snprintf(msg_info, sizeof(msg_info), "Empieza moviendo jugador %u\n\n", turno+1);
		linea_serie_drv_enviar_array( msg_info );
}	





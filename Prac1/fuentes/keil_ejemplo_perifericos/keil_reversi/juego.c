#include <LPC210x.h>

#include "juego.h"
#include "cola_FIFO.h"
#include "temporizador_drv.h"
#include "botones.h"
#include "tablero.h"
#include "conecta_K_2023.h"
#include "linea_serie_drv.h"
#include "Mensaje_t.h"
#include "GPIO_hal.h"
#include <string.h>
#include <stdlib.h>
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

// Funcion auxiliar para determinar si una fila-columna son validas y ademas la celda esta libre.
bool jugada_valida(TABLERO *tablero, const int fila, const int columna);

// Determina si 'inputTrama' es un trama v�lida y ejecuta las acciones asociadas a ella
// En cualquier otro caso muestra un mensaje informativo por l�nea serie
void comprobar_trama(const uint32_t inputTrama);

// Devuelve 1 si 'inputTrama' es una trama jugada
// En cualquier otro caso devuelve 0
bool esTramaJugada(const uint32_t inputTrama);

void inicializarVariables(void);

// Configura una nueva partida e indica c�mo actuar en esta situaci�n
void finalizar_partida(void);


// ***** FUNCIONES ****

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
			
			if( ID_evento == ev_DESPULSACION && auxData == BOTON_1)
			{
				snprintf(msj_info, sizeof(msj_info), "\nMovimiento cancelado\n");
				linea_serie_drv_enviar_array( msj_info );
				
					// cancelar movimiento y sigue turno de mismo jugador
				fila_jugada_por_confirmar = -1;  // eliminar s�mbolo de posici�n pendiente de confirmar
				columna_jugada_por_confirmar = -1; 
				
				imprimir_tablero_linea_serie();
				snprintf(msj_info, sizeof(msj_info), "\nContinua turno de jugador %u\n", turno+1);
				linea_serie_drv_enviar_array( msj_info );
					
				estado = ESPERANDO_JUGADA;	
			}
			else if(ID_evento == ev_LATIDO)
			{
				tacEsperaConfirmacion = clock_get_us();
				if( (tacEsperaConfirmacion - ticEsperaConfirmacion) >= timeout ) 
				{
					snprintf(msj_info, sizeof(msj_info), "\nJugada confirmada\n");
					linea_serie_drv_enviar_array( msj_info );
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
							imprimir_tablero_linea_serie();
								// indicar turno			
							snprintf(msj_info, sizeof(msj_info), "\nTurno de jugador %u\n", turno + 1);
							linea_serie_drv_enviar_array( msj_info );
					}
				}
			}
		break;
	}
}

// jugada_valida
//
// Devuelve 1 si la jugada '_fila'-'_columna' es v�lida
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
// acuerdo al formato de trama definido en "trama.h"
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

// comprobar_trama
//
// Par�metros:
//	inputTrama: trama codificada en un uint32_t
//
// Si 'inputTrama' corresponde a una trama del 
// dominio del juego definidas en "tramas.h"
// se ejecuta las acciones asociadas a ella.
// En cualquier otro caso se env�a por l�nea serie
// un mensaje informativo notificando el tratamiento
void comprobar_trama(const uint32_t inputTrama)
{					
	switch(inputTrama){
	
		case trama_END:			
				// terminar partida
			if( estado != LOBBY){
				gpio_hal_escribir(pin_cmd_no_valido, 1, 0);
				
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
				if( es_trama_jugada_valida(inputTrama) ){	
							// indicar jugada por linea serie
						Mensaje_t msj_info; char sTrama[MAX_LEN_TRAMA];
						gpio_hal_escribir(pin_cmd_no_valido, 1, 0);		// indicar por gpio
						tramaToString(inputTrama, sTrama);
						snprintf(msj_info, sizeof(msj_info), "\nJugada introducida: %s\n", sTrama);
						linea_serie_drv_enviar_array( msj_info );
							// actualizar estado
						imprimir_tablero_linea_serie(); 				
						ticEsperaConfirmacion = clock_get_us(); 
						estado = ESPERANDO_CONFIRMACION; 			
				}
				else {	
						linea_serie_drv_enviar_array("\nJugada no valida\n");
						gpio_hal_escribir(pin_cmd_no_valido, 1, 1);		// indicar por gpio
				}
			}
				// cualquier otro comando fuera del dominio del juego
			else{
					linea_serie_drv_enviar_array("\nComando erroneo\n");
					gpio_hal_escribir(pin_cmd_no_valido, 1, 1); // indicar por gpio
			}
			
		} 
}

// finalizar_partida
//
// Configura una nueva partida y muestra
// por l�nea serie c�mo puede actuar 
// el usuario para jugar de nuevo
void finalizar_partida(void){	
		estado = LOBBY;
		inicializarVariables();		
		imprimir_leyenda_juego();
}

// imprimir_leyenda_juego
//
// Env�a por l�nea serie la leyenda
// del juego para volver a jugar
void imprimir_leyenda_juego(void)
{	
	Mensaje_t leyenda = "\nPara iniciar una nueva partida escriba '$NEW!' o pulse uno de los botones\n";
	linea_serie_drv_enviar_array(leyenda);
}

// imprimir_reglas
//
// Env�a por l�nea serie las reglas del 
// juego que eventualmente ser�n mostradas
// por l�nea serie
void imprimir_reglas(void)
{
	Mensaje_t reglas;
	setMensaje(reglas, "Cada jugador tiene un movimiento en su turno\n"
										 "en el que debe colocar una ficha en\n"
										 "uno de los huecos del tablero\n"
										 "Una vez realizada una jugada dispones de 3s para cancelarla\n");
	linea_serie_drv_enviar_array(reglas);
	
	setMensaje(reglas, "sino, se hace efectiva y se muestra en el tablero\n"
										 "Los movimientos se realizan enviando comandos\n"
                     "a trav�s de la linea serie o presionando los botones 1 y 2.\n");
	linea_serie_drv_enviar_array(reglas);
	
	setMensaje(reglas, "La partida finaliza si ganas o el otro jugador se rinde\n\n"
										 "Comandos para jugar:\n"
										 "'$NEW!' inicia una nueva partida\n"
										 "'$END!' termina la partida en curso por rendici�n\n"
										 "'$X-Y!' indica una jugada, donde X es la fila e Y la columna\n");
	linea_serie_drv_enviar_array(reglas);
	
	setMensaje(reglas, "Botones:\n"
										 "Boton 1 (pin gpio 14) permite cancelar una jugada\n"
										 "valida antes de ser confirmada\n"
										 "Boton 2 (pin gpio 15) termina la partida en curso por rendici�n\n\n"

										 "Para iniciar la partida escriba '$NEW!' o pulse uno de los botones\n"
										 "�Divi�rtete jugando!\n\n");
	linea_serie_drv_enviar_array(reglas);
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
		size_t evento;
	
		switch(condicion_fin){
			
			case VICTORIA:
				sprintf(msg_info, "\nEL JUGADOR %u HA GANADO!!! \n", (!turno)+1);
				linea_serie_drv_enviar_array(msg_info);
				break;
			
			case RENDICION_BOTON:
				sprintf(msg_info, "\nEl jugador %u se ha rendido tras presionar el boton de rendicion\n", turno+1);
				linea_serie_drv_enviar_array(msg_info);
				break;
			
			case RENDICION_COMANDO:
				sprintf(msg_info, "\nEl jugador %u se ha rendido tras introducir comando 'END'\n", turno+1);
				linea_serie_drv_enviar_array(msg_info);
				break;
			
			default:
				// no ha podido llegar hasta aqu�
				break;
		}
		
		linea_serie_drv_enviar_array("Estadisticas de partida\n");
		sprintf(msg_info, "Tiempo total de computo de conecta_K_hay_linea: %" PRIu64 "\n", tTotalHayLinea);
		linea_serie_drv_enviar_array(msg_info);
		sprintf(msg_info, "Tiempo medio computo de conecta_K_hay_linea: %" PRIu64 "\n", tTotalHayLinea/ nVecesHayLinea);
		linea_serie_drv_enviar_array(msg_info);
		sprintf(msg_info, "Tiempo total esperando al jugador: %" PRIu64 "\n", tTotalEsperandoJugada);
		linea_serie_drv_enviar_array(msg_info);
		sprintf(msg_info, "Tiempo medio esperando al jugador: %" PRIu64 "\n", tTotalEsperandoJugada/ nVecesEsperandoJugada);
		linea_serie_drv_enviar_array(msg_info);
		
		linea_serie_drv_enviar_array("\nHistoriograma de eventos en partida:\n");
		// falta historiograma de eventos
		for(evento = 1; evento < N_EVENTOS_REGISTRADOS; evento++){
				sprintf(msg_info, " %s %u\n", getEventoString(evento), FIFO_estadisticas(evento));
				linea_serie_drv_enviar_array(msg_info);
		}
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
// Env�a por l�nea serie el estado actual 
// del tablero que eventualmente es 
// impreso en la l�nea serie
void imprimir_tablero_linea_serie(void)
{
	Mensaje_t msg;
	CELDA celda;
	int offset = 0;
	size_t i,j;
	
	snprintf(msg + offset, sizeof(msg), "-|");
	offset+=2;
		// imprimir fila indicadora de columnas
  for (j = 1; j <= NUM_COLUMNAS; j++) {
     offset += snprintf(msg + offset, sizeof(msg), "%d|", j);
  }
	offset += snprintf(msg + offset, sizeof(msg), "\n");
	
  for (i = 1; i <= NUM_FILAS; i++) {
     offset += sprintf(msg + offset, "%d|", i);
     for (j = 0; j < NUM_COLUMNAS; j++) {
							// a�adir casilla pendiente de confirmaci�n
						if( fila_jugada_por_confirmar+1 == i && columna_jugada_por_confirmar == j){
							offset += snprintf(msg + offset, sizeof(msg), "*|");
							continue;
						}
							// leer valor de tablero
						celda = tablero_leer_celda(&tablero, i-1, j);
						celda = celda_color(celda);
						
							// a�adir caracteres seg�n valor
            if (celda == 0)	
                offset += snprintf(msg + offset, sizeof(msg), " |");
            else if (celda == 1)
                offset += snprintf(msg + offset, sizeof(msg), "B|");
            else if (celda == 2)
                offset += snprintf(msg + offset, sizeof(msg), "N|");
     }
     offset += snprintf(msg + offset, sizeof(msg), "\n"); 
	} 
	offset += snprintf(msg + offset, sizeof(msg), "\n"); 
	linea_serie_drv_enviar_array(msg);
}

// imprimir_vista_inicial_nueva_partida
//
// Env�a por linea serie un mensaje informativo
// del comienzo de la partida, el tablero inicial
// y un mensaje indicando qu� jugador empieza
void imprimir_vista_inicial_nueva_partida(void){
		Mensaje_t msg_info;
		char info[30];
			// mensaje inicio
		setMensaje(msg_info, "--- COMIENZA LA PARTIDA ---\n\n");
		linea_serie_drv_enviar_array(msg_info);
		imprimir_tablero_linea_serie(); 
			// indicar turno (inicia siempre el jugador 1)			
		snprintf(info, sizeof(info), "Empieza moviendo jugador %u\n\n", turno+1);
		setMensaje(msg_info, info);
		linea_serie_drv_enviar_array( msg_info );
}	





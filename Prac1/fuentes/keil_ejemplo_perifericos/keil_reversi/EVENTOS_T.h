#ifndef _EVENTOS_T_H_
#define _EVENTOS_T_H_

#include "tests.h" // definición de estructuras de control de flujo para test unitarios de módulos

// Definición del tipo de datos EVENTO_T
// conjunto de eventos posibles
typedef enum {
    ev_EVENTO_VOID = 0,  			// evento de inicialización
		ev_VISUALIZAR_HELLO = 1,	// visualizar campo 'auxData' en la GPIO
		ev_ALARMAS_OVERFLOW = 2,	// no hay alarmas disponibles ( termina ejecución )
		ev_REVISAR_ALARMAS = 3, 	// planificador tiene que revisar las alarmas
		ev_PULSACION = 4,					// se ha pulsado EINT1 o EINT2
	  ev_POWER_DOWN = 5, 			 	// cambiar a estado power-down del procesador
		ev_VISUALIZAR_CUENTA = 6, // visualizar campo 'auxData' en la GPIO
		ev_LATIDO = 7,						// heartbeat cada 10ms
		ev_DESPULSACION = 9, 			// el botón indicado en 'auxData' ha sido despulsado
		ev_RX_SERIE = 10, 				// recibido comando en linea serie; contenido en 'auxData'
		ev_TX_SERIE = 11, 				// enviado to el mensaje por línea serie
		ev_NUEVA_PARTIDA = 12,		
		ev_TERMINAR_PARTIDA = 13,	
		ev_MOSTRAR_TABLERO = 14,	
		ev_NUEVA_JUGADA = 15,
		
		#if TEST_ALARMAS | TEST_FIFO | TEST_FIFO			// eventos artificiales para test de alarmas
			EVENTO_FICTICIO_1 = -1,
			EVENTO_FICTICIO_2 = -2,
			EVENTO_FICTICIO_3 = -3,
			EVENTO_FICTICIO_4 = -4,
			EVENTO_FICTICIO_5 = -5,
		#endif
	
} EVENTO_T;


#endif // _EVENTOS_T_H_

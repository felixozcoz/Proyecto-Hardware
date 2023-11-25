#ifndef _EVENTOS_T_H_
#define _EVENTOS_T_H_

// Definición del tipo de datos EVENTO_T
// conjunto de eventos posibles
typedef enum {
    EVENTO_VOID = 0,  				// evento de inicialización
		ev_VISUALIZAR_HELLO = 1,	// visualizar campo 'auxData' en la GPIO
		ALARMAS_OVERFLOW = 2,			// no hay alarmas disponibles ( termina ejecución )
		REVISAR_ALARMAS = 3, 			// planificador tiene que revisar las alarmas
		PULSACION = 4,						// se ha pulsado EINT1 o EINT2
	  POWER_DOWN = 5, 			 		// cambiar a estado power-down del procesador
		ev_VISUALIZAR_CUENTA = 6, // visualizar campo 'auxData' en la GPIO
		ev_LATIDO = 7,						// heartbeat cada 10ms
		HELLO_OVERFLOW = 8,				// activar pin de overflow de la GPIO ( termina ejecución )
		DESPULSACION = 9, 				// el botón indicado en 'auxData' ha sido despulsado
		ev_RX_SERIE = 10, 				// recibido comando en linea serie; contenido en 'auxData'
		ev_TX_SERIE = 11, 				// enviado to el mensaje por línea serie
		
		#if TEST_ALARMAS | TEST_FIFO | TEST_FIFO			// eventos artificiales para test de alarmas
			EVENTO_FICTICIO_1 = -1,
			EVENTO_FICTICIO_2 = -2,
			EVENTO_FICTICIO_3 = -3,
			EVENTO_FICTICIO_4 = -4,
			EVENTO_FICTICIO_5 = -5,
		#endif
	
} EVENTO_T;


#endif // _EVENTOS_T_H_

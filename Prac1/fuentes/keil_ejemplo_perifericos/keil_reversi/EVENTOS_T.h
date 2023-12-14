#ifndef _EVENTOS_T_H_
#define _EVENTOS_T_H_

#include "tests.h" // definición de estructuras de control de flujo para test unitarios de módulos
#include <stdlib.h>

static const size_t N_EVENTOS_REGISTRADOS = 11;

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
		ev_DESPULSACION = 8, 			// el botón indicado en 'auxData' ha sido despulsado
		ev_RX_SERIE = 9, 				// recibido comando en linea serie; contenido en 'auxData'
		ev_TX_SERIE = 10, 				// enviado to el mensaje por línea serie
		
		#if TEST_ALARMAS | TEST_FIFO | TEST_FIFO			// eventos artificiales para test de alarmas
			EVENTO_FICTICIO_1 = -1,
			EVENTO_FICTICIO_2 = -2,
			EVENTO_FICTICIO_3 = -3,
			EVENTO_FICTICIO_4 = -4,
			EVENTO_FICTICIO_5 = -5,
		#endif
	
} EVENTO_T;


// Función para obtener la cadena asociada a un evento
__inline const char *getEventoString(EVENTO_T evento) {
    switch (evento) {
        case ev_EVENTO_VOID: return "ev_EVENTO_VOID";
        case ev_VISUALIZAR_HELLO: return "ev_VISUALIZAR_HELLO";
        case ev_ALARMAS_OVERFLOW: return "ev_ALARMAS_OVERFLOW";
        case ev_REVISAR_ALARMAS: return "ev_REVISAR_ALARMAS";
        case ev_PULSACION: return "ev_PULSACION";
        case ev_POWER_DOWN: return "ev_POWER_DOWN";
        case ev_VISUALIZAR_CUENTA: return "ev_VISUALIZAR_CUENTA";
        case ev_LATIDO: return "ev_LATIDO";
        case ev_DESPULSACION: return "ev_DESPULSACION";
        case ev_RX_SERIE: return "ev_RX_SERIE";
        case ev_TX_SERIE: return "ev_TX_SERIE";

#if TEST_ALARMAS | TEST_FIFO | TEST_FIFO
        case EVENTO_FICTICIO_1: return "EVENTO_FICTICIO_1";
        case EVENTO_FICTICIO_2: return "EVENTO_FICTICIO_2";
        case EVENTO_FICTICIO_3: return "EVENTO_FICTICIO_3";
        case EVENTO_FICTICIO_4: return "EVENTO_FICTICIO_4";
        case EVENTO_FICTICIO_5: return "EVENTO_FICTICIO_5";
#endif

        default: return "Unknown Evento";
    }
}


#endif // _EVENTOS_T_H_

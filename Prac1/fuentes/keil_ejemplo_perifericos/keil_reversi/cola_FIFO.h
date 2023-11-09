#ifndef FIFO_H
#define FIFO_H

#include <stdint.h>
#include "GPIO.h"
#include "tests.h"

// Definici�n del tipo de datos EVENTO_T
// conjunto de eventos posibles
typedef enum {
    EVENTO_VOID = 0,  			// Evento de inicializaci�n
		ev_VISUALIZAR_HELLO = 1,	// para visualizaci�n de nuevo hello_world
		ALARMAS_OVERFLOW = 2,		// No hay alarmas disponibles
		REVISAR_ALARMAS = 3, 		// indica que el planificador tiene que revisar las alarmas
		PULSACION = 4,					// se ha pulsado EINT1 o EINT2
	  POWER_DOWN = 5, 			  // pasar/salir a estado power-down
		ev_VISUALIZAR_CUENTA = 6, // indica que se debe visualizar el evento en la gpio
		ev_LATIDO = 7,					// heartbeat cada 10ms
		
		#if TEST_ALARMAS | TEST_FIFO				// eventos artificiales para test de alarmas
			EVENTO_FICTICIO_1 = -1,
			EVENTO_FICTICIO_2 = -2,
			EVENTO_FICTICIO_3 = -3,
			EVENTO_FICTICIO_4 = -4,
			EVENTO_FICTICIO_5 = -5,
		#endif
	
} EVENTO_T;


// ---- Funciones -----

// Inicializa la cola FIFO
// pin_overflow: pin del GPIO para marcar errores
void FIFO_inicializar(const GPIO_HAL_PIN_T pin_overflow);

// Guarda en la cola el evento
//	ID_evento: identificaci�n del evento
//	auxData: informaci�n extra del evento
void FIFO_encolar( EVENTO_T ID_evento, uint32_t auxData);

// Si hay eventos sin procesar devuelve
// la referencia al evento m�s antiguo sin procesar.
// En cualquier otro caso, devuelve 0, que indica que
// la cola est� vac�a
uint8_t FIFO_extraer(EVENTO_T *ID_evento, uint32_t *auxData);

// Devuelve el n�mero de total de veces
// que ese evento con identificador
// ID_EVENTO se ha encolado
// NOTA: el evento EVENTO_VOID devuelve el
// 	total de eventos encolados desde el inicio
uint32_t FIFO_estadisticas(const EVENTO_T ID_evento);

#endif // FIFO_H

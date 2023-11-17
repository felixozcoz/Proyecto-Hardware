#ifndef FIFO_H
#define FIFO_H

#include <stdint.h>
#include "GPIO.h"
#include "tests.h"
#include "io_reserva.h"

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
		
		#if TEST_ALARMAS | TEST_FIFO | TEST_FIFO			// eventos artificiales para test de alarmas
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
//	ID_evento: identificación del evento
//	auxData: información extra del evento
void FIFO_encolar( EVENTO_T ID_evento, uint32_t auxData);

// Si hay eventos sin procesar devuelve
// la referencia al evento más antiguo sin procesar.
// En cualquier otro caso, devuelve 0, que indica que
// la cola está vacía
uint8_t FIFO_extraer(EVENTO_T *ID_evento, uint32_t *auxData);

// Devuelve el número de total de veces
// que ese evento con identificador
// ID_EVENTO se ha encolado
// NOTA: el evento EVENTO_VOID devuelve el
// 	total de eventos encolados desde el inicio
uint32_t FIFO_estadisticas(const EVENTO_T ID_evento);


// getter del tamaño de la cola
unsigned int get_size_FIFO(void);

#endif // FIFO_H

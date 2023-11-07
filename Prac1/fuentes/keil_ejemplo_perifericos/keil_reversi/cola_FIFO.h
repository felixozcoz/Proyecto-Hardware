#ifndef FIFO_H
#define FIFO_H

#include <stdint.h>
#include "GPIO.h"

// Definici�n del tipo de datos EVENTO_T
// conjunto de eventos posibles
typedef enum {
    EVENTO_VOID = 0,  			// Evento de inicializaci�n
		EVENTO_HELLO_WORLD = 1,	// para prueba
		ALARMAS_OVERFLOW = 2,		// No hay alarmas disponibles
		REVISAR_ALARMAS = 3, 		// indica que el planificador tiene que revisar las alarmas
		PULSACION = 4,					// se ha pulsado EINT1 o EINT2
		BOTON_EINT = 5,				 // interrupci�n de EXINT, auxData indica el bot�n
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

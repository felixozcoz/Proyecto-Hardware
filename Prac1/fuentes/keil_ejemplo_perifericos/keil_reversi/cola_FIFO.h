#ifndef FIFO_H
#define FIFO_H

#include <stdint.h>
#include "GPIO.h"

// Definición del tipo de datos EVENTO_T
// conjunto de eventos posibles
typedef enum {
    EVENTO_VOID = 0,  			// Evento de inicialización
    EVENTO_TIMER0 = 1,			// Eventos generados por el timer
		EVENTO_BOTON_EINT0 = 2,     // Evento asíncrono generado por interacción del usuario (E/S) (interrupción EXTINT0)
		EVENTO_HELLO_WORLD = 3,	// para prueba
		ALARMA_OVERFLOW = 4,	// No hay alarmas disponibles
} EVENTO_T;


// ---- Funciones -----

// Inicializa la cola FIFO
// pin_overflow: pin del GPIO para marcar errores
void FIFO_inicializar(GPIO_HAL_PIN_T pin_overflow);

// Guarda en la cola el evento
//	ID_evento: identificación del evento
//	auxData: información extra del evento
void FIFO_encolar(EVENTO_T ID_evento, uint32_t auxData);

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
uint32_t FIFO_estadisticas(EVENTO_T ID_evento);

#endif // FIFO_H

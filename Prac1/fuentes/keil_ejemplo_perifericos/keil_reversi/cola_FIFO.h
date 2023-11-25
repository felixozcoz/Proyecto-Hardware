#ifndef FIFO_H
#define FIFO_H

#include "GPIO_hal.h"
#include "EVENTOS_T.h"
#include <stdio.h>

static const size_t MAX_SIZE_FIFO = 32;

// ---- Funciones -----

// Inicializa la cola FIFO
// pin_inicial: GPIO31 indica overflow en la cola
void FIFO_inicializar(const GPIO_HAL_PIN_T _pin, const GPIO_HAL_PIN_T _num_pin);

// Guarda en la cola el evento
//
// Parámetros:
//	ID_evento: identificación del evento
//	auxData: información extra del evento
void FIFO_encolar( EVENTO_T ID_evento, uint32_t auxData);

// Si hay eventos sin procesar devuelve
// la referencia al evento más antiguo sin procesar.
// En cualquier otro caso, devuelve 0, que indica que
// la cola está vacía
uint8_t FIFO_extraer(EVENTO_T *ID_evento, uint32_t *auxData);

// Devuelve el número de total de veces
// que el evento 'ID_evento' se ha encolado
// NOTA: el evento EVENTO_VOID devuelve el
// 	total de eventos encolados desde el inicio
uint32_t FIFO_estadisticas(const EVENTO_T ID_evento);


#endif // FIFO_H

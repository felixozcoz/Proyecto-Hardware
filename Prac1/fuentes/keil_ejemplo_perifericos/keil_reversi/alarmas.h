// alarmas.h

// Provee funciones para programar alarmas

#ifndef __ALARMAS_H__
#define __ALARMAS_H__

#include <inttypes.h>
#include "cola_FIFO.h"
#include <stdbool.h>

// Número de alarmas activas simultáneamente
static const uint8_t ALARMAS_MAX = 4;

// Estructura que recoge la información de cada alarma
typedef struct {
	bool activa; // true = libre, false = ocupada
		// características de la alarma
	uint32_t inicio; // timestamp en ms de inicio de alarma
	uint32_t retardo;	
	bool periodica; // True = alarma peroódica, False en cualquier otro caso
		// evento programado
	EVENTO_T ID_evento;
	uint32_t auxData;
	
} alarma_t;




// alarma_inicializar
//
// Resetea las alarmas para nuevo uso.
void alarma_inicializar(void);


// alarma_activar
//
// Parámetros:
//	ID_evento: identificador de evento a encolar tras finalizar alarma
//	retardo: duración en ms. Periodicidad si bit de más peso con valor 1, el resto 
//						de bits indican el tiempo. Si 'retardo' = 0 se desactiva alarma
//	auxData: información complementaria del evento
//
// Excepcionalidad: 
//	si se reprograma un determinado evento antes de que finalice
//	la alarma, se reprogramará al nuevo identificador de evento
//
// Inicializa una alarma (periódica o no según se indique) con duración
// igual a 'retardo'. Tras finalizar el tiempo, encola el evento
// 'ID_evento' junto con 'auxData' en la cola de eventos
//
void alarma_activar(EVENTO_T ID_evento, uint32_t retardo, uint32_t auxData);


// alarma_tratar_evento
//
// Comprueba si hay que lanzar algún evento 
// asociado a las alarmas pendientes.
//	Las alarmas periódicas se cancelan 
// tras lanzarse.
//	
void alarma_tratar_evento(const uint32_t periodo_timer1);


#endif // __ALARMAS_H__

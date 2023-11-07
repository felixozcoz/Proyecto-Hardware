// alarmas.h

// Provee funciones para programar alarmas

#ifndef __ALARMAS_H__
#define __ALARMAS_H__

#include <inttypes.h>
#include "cola_FIFO.h"
#include <stdbool.h>

// N�mero de alarmas activas simult�neamente
static const uint8_t ALARMAS_MAX = 4;

// Estructura que recoge la informaci�n de cada alarma
typedef struct {
	bool activa; // true = libre, false = ocupada
		// caracter�sticas de la alarma
	uint32_t inicio; // timestamp en ms de inicio de alarma
	uint32_t retardo;	
	bool periodica; // True = alarma pero�dica, False en cualquier otro caso
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
// Par�metros:
//	ID_evento: identificador de evento a encolar tras finalizar alarma
//	retardo: duraci�n en ms. Periodicidad si bit de m�s peso con valor 1, el resto 
//						de bits indican el tiempo. Si 'retardo' = 0 se desactiva alarma
//	auxData: informaci�n complementaria del evento
//
// Excepcionalidad: 
//	si se reprograma un determinado evento antes de que finalice
//	la alarma, se reprogramar� al nuevo identificador de evento
//
// Inicializa una alarma (peri�dica o no seg�n se indique) con duraci�n
// igual a 'retardo'. Tras finalizar el tiempo, encola el evento
// 'ID_evento' junto con 'auxData' en la cola de eventos
//
void alarma_activar(EVENTO_T ID_evento, uint32_t retardo, uint32_t auxData);


// alarma_tratar_evento
//
// Comprueba si hay que lanzar alg�n evento 
// asociado a las alarmas pendientes.
//	Las alarmas peri�dicas se cancelan 
// tras lanzarse.
//	
void alarma_tratar_evento(const uint32_t periodo_timer1);


#endif // __ALARMAS_H__

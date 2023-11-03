#include "planificador.h"
#include "cola_FIFO.h"
#include "hello_world.h"
#include "power.h"

// Inicilizar cola de eventos
void inicializar_cola_eventos(void) {
	EVENTO_T evento;
	uint32_t auxData;
	FIFO_inicializar(GPIO_OVERFLOW);
	
	// inicializar led y contador para el hell world
	hello_world_inicializar(GPIO_HELLO_WORLD, GPIO_HELLO_WORLD_BITS);
	
	while(1){
		// debería dormir para probar a acumular eventos
		// extraer evento no tratado
		int seguir_tratando = FIFO_extraer(&evento, &auxData);
		
		if(seguir_tratando == 1){
			// tratar dato
			switch(evento){
				case EVENTO_HELLO_WORLD:
					hello_tick_tack();
					break;
				default:
					break;
			}
		} else{
			// detener procesador y reaunador si ocurre interrupción
			power_hal_wait();
		}
	}
}

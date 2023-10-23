#include "planificador.h"
#include "cola_FIFO.h"


// Inicilizar cola de eventos
void inicializar_cola_eventos(void) {
	EVENTO_T evento;
	uint32_t auxData;
	FIFO_inicializar(GPIO_OVERFLOW);
	
	while(1){
		// extraer evento no tratado
		FIFO_extraer(&evento, &auxData);
		
		// tratar dato
		switch(evento){
			case EVENTO_HELLO_WORLD:
				// hello_wold_tick_tack();
			default:
				// nothing
		}
	
	}
}

#include "tests.h"
#include "cola_FIFO.h"

// MAX_SIZE_FIFO está definido en cola_FIFO.h
// que representa la capacidad máxima de la cola

// Test cola FIFO
void test_FIFO(const GPIO_HAL_PIN_T _pin_overflow){
		const EVENTO_T E = ev_EVENTO_VOID;
			// variables
		GPIO_HAL_PIN_T pin_overflow;
		uint32_t est __attribute__((unused));
		uint8_t res __attribute__((unused));
			// aux vars
		EVENTO_T event;
		uint32_t aux;
		int i;
			// set GPIO pins for test
		pin_overflow = _pin_overflow;
				
		FIFO_inicializar(pin_overflow);	// inicializa también los GPIO pins
		
			// Comprobar estadisticas antes de hacer nada
		est = FIFO_estadisticas(E);
		
			// Enconlar y extraer
		FIFO_encolar(E, 0);
		res = FIFO_extraer(&event, &aux);
		est = FIFO_estadisticas(E);
		
			// Extraer con FIFO vacia
		res = FIFO_extraer(&event, &aux);
		est = FIFO_estadisticas(E);
		
			// encolar y extrar n veces, con n >> MAX_SIZE_FIFO
		for(i = 0; i < (MAX_SIZE_FIFO *3); i++){
			FIFO_encolar(E, 0);
			FIFO_extraer(&event, &aux);
		}
		
			// test overflow
		for( i = 0; i < (MAX_SIZE_FIFO + 100); i++)
		FIFO_encolar(E, 0);			
}

#include "tests.h"

#include "cola_FIFO.h"

static const EVENTO_T E = EVENTO_VOID; 

void test_overflow(unsigned int MAX_SIZE)
{
	int i;
	for( i = 0; i < (MAX_SIZE + 100); i++)
		FIFO_encolar(E, 0);			
}

void GPIO_inicializar_test_FIFO(void)
{
		// inicializar gpio
		gpio_hal_iniciar();
			// cambiar sentido de bit de overflow
		gpio_hal_sentido(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS, GPIO_HAL_PIN_DIR_OUTPUT);
}


// Test cola FIFO
void test_FIFO(void){
		// variables
		unsigned int MAX_SIZE = get_size_FIFO();
		uint32_t est __attribute__((unused));
		uint8_t res __attribute__((unused));
		EVENTO_T event;
		uint32_t aux;
		int i;
		
		GPIO_inicializar_test_FIFO();
		
		FIFO_inicializar(GPIO_OVERFLOW);
		
			// Comprobar estadisticas antes de hacer nada
		est = FIFO_estadisticas(E);
		
			// Enconlar y extraer
		FIFO_encolar(E, 0);
		res = FIFO_extraer(&event, &aux);
		est = FIFO_estadisticas(E);
		
			// Extraer con FIFO vacia
		res = FIFO_extraer(&event, &aux);
		est = FIFO_estadisticas(E);
		
			// encolar y extrar n veces, con n >> MAX_SIZE
		for(i = 0; i < (MAX_SIZE *3); i++){
			FIFO_encolar(E, 0);
			FIFO_extraer(&event, &aux);
		}
		
		test_overflow(MAX_SIZE);
}

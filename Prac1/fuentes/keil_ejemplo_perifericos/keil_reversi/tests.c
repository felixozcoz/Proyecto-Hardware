#include <LPC210x.H>                       /* LPC210x definitions */
#include "timer0.h"
#include "Power_management.h"
#include "boton_eint0.h"
#include "temporizador_drv.h"
#include "planificador.h"
#include <inttypes.h>

// Test periférico GPIO
void testGPIO(void)
{
	uint32_t variable_no_usada __attribute__((unused));
	
	// inicializar GPIO
	gpio_hal_iniciar();
	
	// Cambiar sentido de pin Overflow (GPIO31) para escribirlo
	gpio_hal_sentido(GPIO_LED_7, 1, GPIO_HAL_PIN_DIR_OUTPUT);
	
	// escribir para encender pin de Overflow
	gpio_hal_escribir(GPIO_LED_7, 1, 1);
	
	variable_no_usada = gpio_hal_leer(GPIO_LED_7, 1);
}

// Test cola FIFO
void testFIFO(void){
		// variables
		EVENTO_T types_E[4] = { EVENTO_VOID, EVENTO_TIMER0, EVENTO_BOTON_EINT0, EVENTO_HELLO_WORLD};
		uint32_t est __attribute__((unused));
		uint8_t res __attribute__((unused));
		int i;
		EVENTO_T event;
		uint32_t aux;
		
		// inicializar gpio
		gpio_hal_iniciar();
		// cambiar sentido de bit de overflow
		gpio_hal_sentido(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS, GPIO_HAL_PIN_DIR_OUTPUT);
		
		
		// inicializar cola FIFO
		FIFO_inicializar(GPIO_OVERFLOW);
		
		// Comprobar estadisticas antes de hacer nada
		est = FIFO_estadisticas(types_E[0]);
		est = FIFO_estadisticas(types_E[1]);
		est = FIFO_estadisticas(types_E[2]);
		est = FIFO_estadisticas(types_E[3]);
		
	// Enconlar y extraer
		FIFO_encolar(types_E[1], 0);
		res = FIFO_extraer(&event, &aux);
		est = FIFO_estadisticas(types_E[1]);
		
		// Extraer con FIFO vacia
		res = FIFO_extraer(&event, &aux);
		est = FIFO_estadisticas(types_E[1]);
		
		
		// Encolar muiltiples veces
		for(i = 0; i < 8; i++){
			FIFO_encolar(types_E[i%3], 0);
			est = FIFO_estadisticas(types_E[0]);
			est = FIFO_estadisticas(types_E[1]);
			est = FIFO_estadisticas(types_E[2]);
			est = FIFO_estadisticas(types_E[3]);
		}
		
		
		// Extraer multiples veces
		for(i = 0; i < 8; i++){
			res = FIFO_extraer(&event, &aux);
		}
		
		// Llenar FIFO (poner breakpoint 
		// antes del bucle para probar overflow)
		for(i = 0; i < 50; i++){
			FIFO_encolar(types_E[i%3], 0);
			est = FIFO_estadisticas(types_E[1]);
			
		}
		
}

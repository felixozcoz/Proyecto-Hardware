#include <inttypes.h>
#include "hello_world.h"
#include "io_reserva.h"
#include "alarmas.h"

// Contador para indicar el led
static uint8_t contador = 0;

// Localización y número de bits de leds en GPIO
static GPIO_HAL_PIN_T gpio_inicial = 0;
static uint8_t num_bits = 0;

void hello_world_inicializar(GPIO_HAL_PIN_T _gpio_inicial, uint8_t _num_bits) {
	gpio_inicial = _gpio_inicial;
	num_bits = _num_bits;
	
	// inicilizar gpio
	gpio_hal_iniciar();
	// cambiar sentido de bits correspondientes como output
	gpio_hal_sentido(gpio_inicial, num_bits, GPIO_HAL_PIN_DIR_OUTPUT);
	// cambiar sentido de bit de overflow
	gpio_hal_sentido(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS, GPIO_HAL_PIN_DIR_OUTPUT);
	// reinicializar contador
	contador = 0;
	
	// cada 10ms genera un evento ev_LATIDO
	alarma_activar(ev_LATIDO, 10, 0);
}


void hello_world_tratar_evento(void)
{
	if(contador == 0xFF){
		FIFO_encolar(HELLO_OVERFLOW, 0);
	}
	contador++;
	contador = contador % 256;
	
	FIFO_encolar(ev_VISUALIZAR_HELLO, 0);
}

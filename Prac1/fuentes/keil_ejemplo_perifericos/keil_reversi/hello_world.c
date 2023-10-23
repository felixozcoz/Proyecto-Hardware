#include "GPIO.h"
#include <inttypes.h>

// Contador para indicar el led
static uint8_t contador = 0;

// Localizaci�n y n�mero de bits de leds en GPIO
static GPIO_HAL_PIN_T gpio_inicial = 0;
static uint8_t num_bits = 0;

void hello_world_inicializar(GPIO_HAL_PIN_T _gpio_inicial, uint8_t _num_bits) {
	gpio_inicial = _gpio_inicial;
	num_bits = _num_bits;
	
	// inicilizar gpio
	gpio_hal_iniciar();
	// cambiar sentido de bits correspondientes como output
	gpio_hal_sentido(gpio_inicial, num_bits, GPIO_HAL_PIN_DIR_OUTPUT);

	contador = 0;
}


void hello_tick_tack() {
	contador++;
	contador = contador % 256;
	
	gpio_hal_escribir(gpio_inicial, num_bits, contador);
}
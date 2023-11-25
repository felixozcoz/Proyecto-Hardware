#include "visualizar.h"

// Localización y número de bits de leds en GPIO
static GPIO_HAL_PIN_T gpio_inicial = 0;
static uint8_t num_bits = 0;


void inicializar_visualizar(GPIO_HAL_PIN_T _gpio_inicial, uint8_t _num_bits ){
	gpio_inicial = _gpio_inicial;
	num_bits = _num_bits;
	
	// cambiar sentido de bits correspondientes como output
	gpio_hal_sentido(_gpio_inicial, _num_bits, GPIO_HAL_PIN_DIR_OUTPUT);
}

void visualizar_cuenta(const uint32_t cuenta)
{
	gpio_hal_escribir(gpio_inicial, num_bits, cuenta);
}

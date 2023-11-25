#include "tests.h"

#include <inttypes.h>

#include "GPIO_hal.h"
#include "io_reserva.h"


// Test periférico GPIO
void test_GPIO(void)
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

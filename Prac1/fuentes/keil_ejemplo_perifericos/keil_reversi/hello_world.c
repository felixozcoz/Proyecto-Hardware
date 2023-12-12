#include <inttypes.h>

#include "hello_world.h"
#include "alarmas.h"
#include "cola_FIFO.h"


// Contador para indicar el led
static uint8_t contador = 0;

// Pin inicial de la GPIO y 
// número de pins desde ese, respectivamente
static GPIO_HAL_PIN_T pin_inicial;
static GPIO_HAL_PIN_T num_pins;


void hello_world_inicializar(const GPIO_HAL_PIN_T _pin_inicial, const GPIO_HAL_PIN_T _num_pins) 
{
	contador = 0;
		// set GPIO pins
	pin_inicial = _pin_inicial;
	num_pins = _num_pins;
	gpio_hal_sentido(pin_inicial, num_pins, GPIO_HAL_PIN_DIR_OUTPUT);
	
	alarma_activar(ev_LATIDO, 0x8000000A, 0); // heartbeat (periódico) cada 10ms
}


void hello_world_tratar_evento(void)
{
	contador++;
	contador = contador % 256;
	
	FIFO_encolar(ev_VISUALIZAR_HELLO, contador);
}

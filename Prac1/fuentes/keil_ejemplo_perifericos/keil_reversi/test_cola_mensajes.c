#include "cola_mensajes.h"
#include "linea_serie_drv.h"
#include "tests.h"
#include <string.h>

void test_cola_mensajes(const GPIO_HAL_PIN_T _pin_overflow){
		// mensajes de prueba
	Mensaje_t m1 = "primer mensaje\n",
						m2 = "segundo mensaje\n",
						m3 = "tercero mensaje\n";

	// inicializar módulos usados en test
	inicializar_cola_mensajes(_pin_overflow);
		// set gpio
	gpio_hal_sentido(_pin_overflow, 1, GPIO_HAL_PIN_DIR_OUTPUT);
	
		// encolar
	encolar_msg(m1);
	while( estaVacia_msg() ) while(1);
	
		// desencolar
	desencolar_msg(m1);
	while( !estaVacia_msg() ) while(1);
	
		// observar mensajes desencolados
	linea_serie_drv_enviar_array(m1);
	linea_serie_drv_enviar_array(m2);
	linea_serie_drv_enviar_array(m3);
	
		// overflow
	while(1){
		encolar_msg(m1);
		encolar_msg(m2);
		encolar_msg(m3);
	}
}

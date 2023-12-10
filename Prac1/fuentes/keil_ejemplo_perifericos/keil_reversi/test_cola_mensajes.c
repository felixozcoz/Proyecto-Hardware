#include "cola_mensajes.h"
#include "linea_serie_drv.h"
#include "tests.h"
#include <string.h>

void test_cola_mensajes(const GPIO_HAL_PIN_T _pin_overflow){
		// mensajes de prueba
	Mensaje_t m1, m2, m3;
	char* c1 = "primer mensaje\n";
	char* c2 = "segundo mensaje\n";
	char* c3 = "tercero mensaje\n";
	
	strncpy(m1, c1, strlen(c1) +1);
	strncpy(m2, c2, strlen(c2) +1);
	strncpy(m3, c3, strlen(c3) +1);
		// inicializar módulos usados en test
	inicializar_cola_mensajes(_pin_overflow);
		// set gpio
	gpio_hal_sentido(_pin_overflow, 1, GPIO_HAL_PIN_DIR_OUTPUT);
	
		// encolar
	encolar_msg(m1);
	while( estaVacia_msg() ) while(1);
	
		// desencolar
	desencolar_msg(&m1);
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

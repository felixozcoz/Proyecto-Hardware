#ifndef __TEST_H__
#define __TEST_H__

// Descripción del fichero:
// 	Contiene definiciones de control de flujo para ejecución
// 	de test unitarios de módulos y demostradores

#include "GPIO_hal.h"
#include "io_reserva.h"
	
#define TEST_FIFO    				0
#define TEST_GPIO    				0
#define TEST_ALARMAS 				0
#define TEST_BOTONES 				0
#define TEST_CONSUMO 				0
#define TEST_WATCHDOG 			0
#define TEST_COLA_MENSAJES 	1
#define JUEGO  							0	
#define HELLO_WORLD_DEMO 		0

	
	
void test_FIFO(const GPIO_HAL_PIN_T _pin_overflow);
void test_GPIO(void);
void test_alarmas(void);
void test_botones(void); // TODO
void test_consumo(void); // TODO
void test_cola_mensajes(const GPIO_HAL_PIN_T _pin_overflow);

#endif // __TEST_H__

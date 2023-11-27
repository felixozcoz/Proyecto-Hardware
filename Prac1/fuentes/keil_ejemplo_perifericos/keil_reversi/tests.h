#ifndef __TEST_H__
#define __TEST_H__

// Descripción del fichero:
// 	Contiene definiciones de control de flujo para ejecución
// 	de test unitarios de módulos y demostradores

#include "GPIO_hal.h"
#include "io_reserva.h"
	
#define TEST_FIFO    1
#define TEST_GPIO    1
#define TEST_ALARMAS 1
#define TEST_BOTONES 1
#define TEST_CONSUMO 1
#define TEST_WATCHDOG 1
#define JUEGO  1
#define HELLO_WORLD_DEMO 1
	
	
void test_FIFO(const GPIO_HAL_PIN_T _pin, const GPIO_HAL_PIN_T _num_pins);
void test_GPIO(void);
void test_alarmas(void);
void test_botones(void); // TODO
void test_consumo(void); // TODO

#endif // __TEST_H__

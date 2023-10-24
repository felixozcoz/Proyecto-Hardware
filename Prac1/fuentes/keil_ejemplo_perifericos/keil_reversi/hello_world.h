#ifndef HELLO_WORLD_H
#define HELLO_WORLD_H

#include "GPIO.h"

void hello_world_inicializar(GPIO_HAL_PIN_T _gpio_inicial, uint8_t _num_bits);
void hello_tick_tack(void);

#endif // HELLO_WORLD_H

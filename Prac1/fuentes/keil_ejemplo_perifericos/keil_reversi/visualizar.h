#ifndef __VISUALIZAR_H__
#define __VISUALIZAR_H__

#include <inttypes.h>
#include "GPIO.h"


void inicializar_visualizar(GPIO_HAL_PIN_T _gpio_inicial, uint8_t _num_bits );

void visualizar_cuenta(const uint32_t cuenta);

#endif // __VISUALIZAR_H__

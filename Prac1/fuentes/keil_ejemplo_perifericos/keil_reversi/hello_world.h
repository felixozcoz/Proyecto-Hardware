#ifndef HELLO_WORLD_H
#define HELLO_WORLD_H

#include "GPIO_hal.h"

// hello_world_inicializar
//
// inicializa las variables necesaras y los pines necesarios de la GPIO
void hello_world_inicializar(const GPIO_HAL_PIN_T _pin_inicial, const GPIO_HAL_PIN_T _num_pins);

// hello_world_tratar_evento
//
// En cada llamada a esta función incrementa 
// de forma unitaria y monótona su contador
// y encola el evento ev_VISUALIZAR_HELLO_WORLD
// para visualizar el contador en la GPIO
// NOTA: tamaño del contador restringido a 8bits
void hello_world_tratar_evento(void);

#endif // HELLO_WORLD_H

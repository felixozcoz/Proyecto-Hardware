#ifndef TEMPORIZADOR_HAL_H
#define TEMPORIZADOR_HAL_H

#include <stdint.h>

// 	TIMER/TEMPORIZADOR 0

// inicializar temporizador0
void temporizador0_hal_iniciar();

// empezar temporizador0
void temporizador0_hal_empezar();


// lee el tiempo que lleva contando el temporizador0 
// desde la última vez que se ejecutó 
// temporizador0_hal_empezar y lo devuelve en ticks.
uint64_t temporizador0_hal_leer();

// detiene el temporizador0 y 
// devuelve el tiempo transcurrido en ticks
// desde el último temporizador0_hal_empezar.
uint64_t temporizador0_hal_parar();


// 	TIMER/TEMPORIZADOR 1

// inicializar temporizador1
void temporizador1_hal_iniciar();

// empezar temporizador1
void temporizador1_hal_empezar();

// lee el tiempo que lleva contando el temporizador1 
// desde la última vez que se ejecutó 
// temporizador1_hal_empezar  y lo devuelve en ticks.
uint64_t temporizador1_hal_leer();

// detiene el temporizador1 y 
// devuelve el tiempo transcurrido en ticks
// desde el último temporizador1_hal_empezar.
uint64_t temporizador1_hal_parar();



#endif /* TEMPORIZADOR_HAL_H */

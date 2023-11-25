#ifndef TEMPORIZADOR_HAL_H
#define TEMPORIZADOR_HAL_H

#include <stdint.h>

// Constante que convierte de tics a ms
extern const float temporizador_hal_ticks2us;

// TIMER 0

// inicializar contador 
void temporizador0_hal_iniciar(void);

// empezar contador 
void temporizador0_hal_empezar(void);

// lee el tiempo que lleva contando el temporizador0 
// desde la última vez que se ejecutó 
// temporizador0_hal_empezar y lo devuelve en ticks.
uint64_t temporizador0_hal_leer(void);

// detiene el timer0 y 
// devuelve el tiempo transcurrido en ticks
// desde el último temporizador_hal_empezar.
uint64_t temporizador0_hal_parar(void);

// TIMER 1

// Función que programa el reloj para que llame a la función de 
// callback cada periodo. El peridod se indica en ms.
// Si el periodo es 0 se para el temporizador
void temporizador_hal_reloj (uint32_t periodo, void (*funcion_callback)());

// inicializar contador 
void temporizador1_hal_iniciar(void);

// empezar contador 
void temporizador1_hal_empezar(void);

// lee el tiempo que lleva contando el temporizador1
// desde la última vez que se ejecutó 
// temporizador0_hal_empezar y lo devuelve en ticks.
uint64_t temporizador1_hal_leer(void);

// detiene el timer1 y 
// devuelve el tiempo transcurrido en ticks
// desde el último temporizador_hal_empezar.
uint64_t temporizador1_hal_parar(void);

#endif /* TEMPORIZADOR_HAL_H */

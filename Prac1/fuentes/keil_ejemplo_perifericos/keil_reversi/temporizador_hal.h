#ifndef TEMPORIZADOR_HAL_H
#define TEMPORIZADOR_HAL_H

#include <stdint.h>

// Utiliza Timer 0 de LPC2105

extern const float temporizador_hal_ticks2us;


// inicializar contador
void temporizador_hal_iniciar(void);

// empezar contador
void temporizador_hal_empezar(void);

// lee el tiempo que lleva contando el temporizador0 
// desde la última vez que se ejecutó 
// temporizador0_hal_empezar y lo devuelve en ticks.
uint64_t temporizador_hal_leer(void);

// detiene el timer0 y 
// devuelve el tiempo transcurrido en ticks
// desde el último temporizador_hal_empezar.
uint64_t temporizador_hal_parar(void);



#endif /* TEMPORIZADOR_HAL_H */

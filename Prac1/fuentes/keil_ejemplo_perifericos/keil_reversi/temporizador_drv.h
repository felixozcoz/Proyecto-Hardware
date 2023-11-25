#ifndef TEMPORIZADOR_DRV_H
#define TEMPORIZADOR_DRV_H

#include "EVENTOS_T.h"
#include <inttypes.h>

void temporizador_drv_iniciar(void);
void temporizador_drv_empezar(void);
uint64_t temporizador_drv_leer(void);
uint64_t temporizador_drv_parar(void);

// programa el reloj (timer 1) para provocar una
// interrupción cada 'periodo' (en ms) en la que
// invoca a la función de callback
void temporizador_drv_reloj (uint32_t periodo, void (*funcion_callback)(), EVENTO_T ID_evento);
void temporizador_drv_callback_reloj(void);
uint64_t temporizador1_parar_drv(void);
// llamada al sistema que devuelve el tiempo
// del reloj
uint32_t __swi(0) clock_get_us(void);

#endif // TEMPORIZADOR_DRV_H

#ifndef TEMPORIZADOR_DRV_H
#define TEMPORIZADOR_DRV_H

#include <stdint.h>
#include "cola_FIFO.h"

void temporizador_drv_iniciar(void);
void temporizador_drv_empezar(void);
uint64_t temporizador_drv_leer(void);
uint64_t temporizador_drv_parar(void);
void temporizador_drv_reloj (uint32_t periodo, void (*funcion_encolar_evento)(), EVENTO_T ID_evento);
void temporizador_drv_callback_reloj(void);
uint64_t temporizador1_parar_drv(void);
uint64_t temporizador1_leer_drv(void);

#endif // TEMPORIZADOR_DRV_H

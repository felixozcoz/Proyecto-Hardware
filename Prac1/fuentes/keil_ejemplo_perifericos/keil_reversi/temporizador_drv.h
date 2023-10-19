#ifndef TEMPORIZADOR_DRV_H
#define TEMPORIZADOR_DRV_H

#include <stdint.h>

void temporizador_drv_iniciar();
void temporizador_drv_empezar();
uint64_t temporizador_drv_leer();
uint64_t temporizador_drv_parar();

#endif // TEMPORIZADOR_DRV_H

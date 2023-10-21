#ifndef TEMPORIZADOR_DRV_H
#define TEMPORIZADOR_DRV_H

#include <stdint.h>

void temporizador_drv_iniciar(void);
void temporizador_drv_empezar(void);
uint64_t temporizador_drv_leer(void);
uint64_t temporizador_drv_parar(void);

#endif // TEMPORIZADOR_DRV_H

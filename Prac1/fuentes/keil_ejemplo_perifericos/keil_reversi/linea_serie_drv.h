#ifndef _LINEA_SERIE_DRV_H_
#define _LINEA_SERIE_DRV_H_

// Módulo interface de periférico UART0

#include "linea_serie_hal.h"
#include <inttypes.h>
#include "cola_FIFO.h"

void iniciar_serial(void);
// fsm que gestiona
void gestor_serial(void);
void linea_serie_drv_enviar_array(char *buff);


#endif // _LINEA_SERIE_DRV_H_

#ifndef _LINEA_SERIE_HAL_H_
#define _LINEA_SERIE_HAL_H_

#include <LPC210X.H>
#include <stddef.h>

void iniciar_serial_hal( void(*_funcion_callback_transmision)(), void(*_funcion_callback_recepcion)() );
void sendchar_serie(char ch);
int getchar_serie(void);

#endif // _LINEA_SERIE_HAL_H_

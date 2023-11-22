#ifndef _LINEA_SERIE_HAL_H_
#define _LINEA_SERIE_HAL_H_

#include <LPC210X.H>

// Módulo de gestión de periférico UART0
// a bajo nivel

void init_serial(void);
void sendchar(int ch);
int getchar(void);





#endif // _LINEA_SERIE_HAL_H_

#ifndef WD_H
#define WD_H


#include <stdio.h>
#include <LPC210X.H> // LPC21XX Peripheral Registers
#include <inttypes.h>
#include "SWI_llamadas.h"


void WD_hal_inicializar(uint32_t sec);

void WD_hal_feed(void);

void WD_hal_test(uint32_t sec);


#endif // WD_H

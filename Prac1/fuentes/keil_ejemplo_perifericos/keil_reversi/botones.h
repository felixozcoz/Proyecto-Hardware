// botones.h
//
// Interface de botones simulados mediante
// interrupciones externas (EINT1 y EINT2)

#ifndef __BOTONES_H__
#define	__BOTONES_H__

#include "int_external_hal.h"

// EINT1

void eint1_iniciar_drv (void);
unsigned int eint1_leer_nueva_pulsacion_drv (void);
void eint1_clear_nueva_pulsacion_drv (void);
unsigned int eint1_leer_cuenta_drv (void);

// EINT2

void eint2_iniciar_drv (void);
unsigned int eint2_leer_nueva_pulsacion_drv( void);
void eint2_clear_nueva_pulsacion_drv (void);
unsigned int eint2_leer_cuenta_drv (void);


#endif // __BOTONES_H__

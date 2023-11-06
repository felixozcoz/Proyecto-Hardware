#include "botones.h"
#include <LPC210X.H> // LPC21XX Peripheral Registers

// EINT1 (External Interrupt 1)

void eint1_iniciar_drv (void)
{ eint1_iniciar_hal(); }
		
// leer pulsación
unsigned int eint1_leer_nueva_pulsacion_drv (void)
{ return eint1_leer_nueva_pulsacion_hal(); }

// nueva pulsacion
void eint1_clear_nueva_pulsacion_drv (void)
{ eint1_clear_nueva_pulsacion_hal(); }

// leer cuenta
unsigned int eint1_leer_cuenta_drv (void)
{ return eint1_leer_cuenta_hal(); }


// EINT2 (External Interrupt 2)

void eint2_iniciar_drv (void)
{ eint2_iniciar_drv(); }

unsigned int eint2_leer_nueva_pulsacion_drv( void)
{ return eint2_leer_nueva_pulsacion_hal(); }

void eint2_clear_nueva_pulsacion_drv (void)
{ eint2_clear_nueva_pulsacion_hal(); }

unsigned int eint2_leer_cuenta_drv (void)
{ return eint2_leer_cuenta_hal(); }

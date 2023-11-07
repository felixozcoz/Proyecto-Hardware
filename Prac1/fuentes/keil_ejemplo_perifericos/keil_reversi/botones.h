// botones.h
//
// Interface de botones simulados mediante
// interrupciones externas (EINT1 y EINT2)

#ifndef __BOTONES_H__
#define	__BOTONES_H__

#include "int_external_hal.h"
#include <inttypes.h>

// EINT1

void eint1_iniciar_drv (void);
unsigned int eint1_leer_nueva_pulsacion_drv (void);
void eint1_clear_nueva_pulsacion_drv (void);
unsigned int eint1_leer_cuenta_drv (void);
void eint1_reactivar_interrupciones_drv(void);
unsigned int eint1_comprobar_pulsado_drv(void);
void eint1_gestionar_pulsacion(void);

// EINT2

void eint2_iniciar_drv (void);
unsigned int eint2_leer_nueva_pulsacion_drv( void);
void eint2_clear_nueva_pulsacion_drv (void);
unsigned int eint2_leer_cuenta_drv (void);
void eint2_reactivar_interrupciones_drv(void);
unsigned int eint2_comprobar_pulsado_drv(void);
void eint2_gestionar_pulsacion(void);
	
// Función para gestión de pulsación
void pulsacion_detectada(const uint8_t id_boton);
	
#endif // __BOTONES_H__

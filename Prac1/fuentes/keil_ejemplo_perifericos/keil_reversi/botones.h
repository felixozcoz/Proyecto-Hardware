#ifndef __BOTONES_H__
#define	__BOTONES_H__

#include "boton_eint1.h"
#include "boton_eint2.h"



// Funciones:
//	ISR 
// 	iniciar
//	leer pulsación ??
//  limpiar nueva pulsación
// falta alguna más??


// EINT1

void eint1_ISR (void) __irq;    
void iniciar_eint1 (void);
unsigned int eint1_leer_nueva_pulsacion (void);
void eint1_clear_nueva_pulsacion (void);


// EINT2

void eint2_ISR (void) __irq;    
void iniciar_eint2 (void);
unsigned int eint2_leer_nueva_pulsacion( void);
void eint2_clear_nueva_pulsacion (void);



#endif // __BOTONES_H__
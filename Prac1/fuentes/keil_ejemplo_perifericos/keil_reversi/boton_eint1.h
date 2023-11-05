// boton_eint1.h
//
// Botón que simula interrupciones externas al SoC

#ifndef __BOTON_EINT1__
#define __BOTON_EINT1__


void eint1_iniciar (void);
unsigned int eint1_leer_nueva_pulsacion (void);
void eint1_clear_nueva_pulsacion (void);


#endif // __BOTON_EINT1__

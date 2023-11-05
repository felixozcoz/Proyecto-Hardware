// boton_eint2.h
//
// Botón que simula interrupciones externas al SoC

#ifndef __BOTON_EINT2__
#define __BOTON_EINT2__


void eint2_iniciar (void);
unsigned int eint2_leer_nueva_pulsacion (void);
void eint2_clear_nueva_pulsacion (void);


#endif // __BOTON_EINT2__

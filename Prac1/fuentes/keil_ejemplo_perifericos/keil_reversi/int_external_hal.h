#ifndef __INT_EXTERNAL_HAL__
#define __INT_EXTERNAL_HAL__


// EINT 1

//void eint1_ISR (void) __irq;
void eint1_iniciar_hal (void);
unsigned int eint1_leer_nueva_pulsacion_hal (void);
void eint1_clear_nueva_pulsacion_hal (void);
unsigned int eint1_leer_cuenta_hal (void);

// EINT 2

//void eint2_ISR (void) __irq;
void eint2_iniciar_hal (void);
unsigned int eint2_leer_nueva_pulsacion_hal (void);
void eint2_clear_nueva_pulsacion_hal (void);
unsigned int eint2_leer_cuenta_hal (void);

#endif // __INT_EXTERNAL_HAL__

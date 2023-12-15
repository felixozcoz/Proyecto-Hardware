#ifndef __SWI_HAL_
#define __SWI_HAL_

#include <inttypes.h>

// Almacena el valor del bit I (bit 7) del SPSR
// antes de modificar SPSR
extern volatile int8_t bit_irq;		// declarado en "SWI_llamadas.c"
	
// Habilita las interrupciones IRQ
void __swi(0xFF) enable_irq (void);

// Deshabilita las interrupciones IRQ
void __swi(0xFE) disable_irq(void);

// Habilita las interrupciones FIQ
void __swi(0xFD) disable_fiq(void);

// Devuelve el valor del bit I (bit 7) del SPSR
uint8_t __swi(0xFC) read_IRQ_bit(void);

#endif // __SWI_HAL_

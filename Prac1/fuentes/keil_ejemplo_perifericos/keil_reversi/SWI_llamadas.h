#ifndef __SWI_LLAMADAS_
#define __SWI_LLAMADAS_

#include <inttypes.h>

// Almacena el valor del bit I del SPSR
// antes de modificar SPSR
extern int8_t bit_irq;		// declarado en "SWI_llamadas.c"
	
// activar interrupciones irq en el registro de estado
void __swi(0xFF) enable_irq (void);

// desactivar interrupciones irq en el registro de estado
void __swi(0xFE) disable_irq(void);

// desactivar interrupciones ifq en el registro de estado
void __swi(0xFD) disable_fiq(void);

// leer bit I del SPSR (bit I es IRQ interrupt disable bit)
int __swi(0xFC) read_IRQ_bit(void);

#endif // __SWI_LLAMADAS_

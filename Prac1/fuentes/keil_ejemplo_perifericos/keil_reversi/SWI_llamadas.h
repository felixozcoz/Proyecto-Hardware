// activar interrupciones irq en el registro de estado
void __swi(0xFF) enable_irq (void);

// desactivar interrupciones irq en el registro de estado
void __swi(0xFE) disable_irq(void);

// desactivar interrupciones ifq en el registro de estado
void __swi(0xFD) disable_fiq(void);

//
int __swi(0xFC) read_IRQ_bit(void);

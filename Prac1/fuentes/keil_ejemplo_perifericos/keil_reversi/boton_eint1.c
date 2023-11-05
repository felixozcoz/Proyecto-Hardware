#include "boton_eint1.h"
#include <LPC210X.H>                            // LPC21XX Peripheral Registers

// variable para comprobar que se hacen las interrupciones que deberían hacerse
static volatile unsigned int eint1_count = 0;
// variable que se activa al detectar una nueva pulsación
static volatile int eint1_nueva_pulsacion = 0;

void eint1_ISR (void) __irq {
	eint1_count++;
	EXTINT =  EXTINT | 1;        // clear interrupt flag        
	VICVectAddr = 0;             // Acknowledge Interrupt
	eint1_nueva_pulsacion = 1;
}

void eint1_clear_nueva_pulsacion(void){
	eint1_nueva_pulsacion = 0;
};

unsigned int eint1_read_nueva_pulsacion(void){
	return eint1_nueva_pulsacion;
};

unsigned int eint1_read_count(void){
	return eint1_count;
};

void eint1_init (void) {
// NOTA: según el manual se puede configurar cómo se activan las interrupciones: por flanco o nivel, alta o baja. 
// Se usarían los registros EXTMODE y EXTPOLAR. 
// Sin embargo parece que el simulador los ignora y no aparecen en la ventana de ocnfiguración de EXT Interrupts
// configure EXTINT0 active if a rising-edge is detected
//	EXTMODE 	=	1; //1 edge, 0 level
//	EXTPOLAR	=	1; // 1 high, rising-edge; 0 low, falling-edge
//  prueba = EXTMODE;
	eint1_nueva_pulsacion = 0;
	eint1_count = 0;
	EXTINT =  EXTINT | 2;        // clear interrupt flag     	
	// configuration of the IRQ slot number 2 of the VIC for EXTINT0
	VICVectAddr2 = (unsigned long)eint1_ISR;          // set interrupt vector in 0
    // 0x20 bit 5 enables vectored IRQs. 
		// 14 is the number of the interrupt assigned. Number 14 is the eint1 (see table 40 of the LPC2105 user manual  
	PINSEL1 		= PINSEL1 & 0xfffffffC;	//Sets bits 0 and 1 to 0
	PINSEL1 		= PINSEL1 | 1;					//Enable the EXTINT0 interrupt
	VICVectCntl2 = 0x20 | 14;                   
  VICIntEnable = VICIntEnable | 0x00004000;                  // Enable EXTINT0 Interrupt
}

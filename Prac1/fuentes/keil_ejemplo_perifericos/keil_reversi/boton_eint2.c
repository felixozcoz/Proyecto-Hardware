#include "boton_eint2.h"
#include <LPC210X.H>                            // LPC21XX Peripheral Registers

// variable para comprobar que se hacen las interrupciones que deberían hacerse
static volatile unsigned int eint2_count = 0;
// variable que se activa al detectar una nueva pulsación
static volatile int eint2_nueva_pulsacion = 0;

void eint2_ISR (void) __irq {
	eint2_count++;
	
	VICIntEnClr = VICIntEnClr | (1 << 16); // disable eint2 interrupt
	EXTINT =  EXTINT | 4;   // clear interrupt flag        
	VICVectAddr = 0;        // Acknowledge Interrupt
	
	eint2_nueva_pulsacion = 1;
}

void eint2_clear_nueva_pulsacion(void){
	eint2_nueva_pulsacion = 0;
};

unsigned int eint2_read_nueva_pulsacion(void){
	return eint2_nueva_pulsacion;
};

unsigned int eint2_read_count(void){
	return eint2_count;
};

void eint2_iniciar (void) {
// NOTA: según el manual se puede configurar cómo se activan las interrupciones: por flanco o nivel, alta o baja. 
// Se usarían los registros EXTMODE y EXTPOLAR. 
// Sin embargo parece que el simulador los ignora y no aparecen en la ventana de ocnfiguración de EXT Interrupts
// configure EXTINT0 active if a rising-edge is detected
//	EXTMODE 	=	1; //1 edge, 0 level
//	EXTPOLAR	=	1; // 1 high, rising-edge; 0 low, falling-edge
//  prueba = EXTMODE;
	eint2_nueva_pulsacion = 0;
	eint2_count = 0;
	EXTINT =  EXTINT | 4;        // clear interrupt flag     	
	// configuration of the IRQ slot number 2 of the VIC for EXTINT0
	VICVectAddr2 = (unsigned long)eint2_ISR;          // set interrupt vector in 0
    // 0x20 bit 5 enables vectored IRQs. 
		// 14 is the number of the interrupt assigned. Number 14 is the eint2 (see table 40 of the LPC2105 user manual  
	PINSEL1 		= PINSEL1 & 0xfffffffC;	//Sets bits 0 and 1 to 0
	PINSEL1 		= PINSEL1 | 1;					//Enable the EXTINT0 interrupt
	VICVectCntl2 = 0x20 | 16;                   
  VICIntEnable = VICIntEnable | 0x00004000;                  // Enable EXTINT0 Interrupt
}

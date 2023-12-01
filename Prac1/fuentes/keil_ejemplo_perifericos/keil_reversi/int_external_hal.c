// int_external_hal.c

#include "int_external_hal.h"
#include "botones.h"
#include <LPC210X.h>
#include <stddef.h>

// variables para comprobar que se hacen las 
// interrupciones que deberían hacerse
static volatile unsigned int eint1_cuenta = 0;
static volatile unsigned int eint2_cuenta = 0;

// variables que se activa al detectar una 
// nueva pulsación
static volatile unsigned int eint1_nueva_pulsacion = 0;
static volatile unsigned int eint2_nueva_pulsacion = 0;

// Función de callback para gestión de pulsación
static void(*func_callback_eint1)(uint8_t) = NULL;
static void(*func_callback_eint2)(uint8_t) = NULL;

// EINT 1

void eint1_ISR (void) __irq
{
	VICIntEnClr = VICIntEnClr | 0x00008000; // disable eint1 interrupt
	EXTINT =  EXTINT | 2;   // clear interrupt flag        
	VICVectAddr = 0;        // Acknowledge Interrupt
	
	eint1_cuenta++;
	eint1_nueva_pulsacion = 1;
	
		// call módulo gestión de ev_PULSACIONes 
	func_callback_eint1(1);
}


unsigned int eint1_leer_nueva_pulsacion_hal (void)
{ 
	return eint1_nueva_pulsacion; 
}


void eint1_clear_nueva_pulsacion_hal (void)
{ 
	eint1_nueva_pulsacion = 0; 
}


unsigned int eint1_leer_cuenta_hal (void)
{ 
	return eint2_cuenta; 
}

void eint1_reactivar_interrupciones_hal(void)
{
	// write 1 on EINT1 interrupt bit (15) to enable
	VICIntEnable = VICIntEnable | 0x00008000;
}

// Comprueba que eint2 sigue pulsado
unsigned int eint1_comprobar_flag(void)
{
	EXTINT = EXTINT | 2;	// limpiar flag 
	return EXTINT & 2 ? 1 : 0;	// comprobar valor del flag
}

void eint1_iniciar_hal (void(*func_callback)(uint8_t))
{
	// registrar función de callback para interrupción
	func_callback_eint1 = func_callback;
	
	// NOTA: según el manual se puede configurar cómo se activan las interrupciones: por flanco o nivel, alta o baja. 
	// Se usarían los registros EXTMODE y EXTPOLAR. 
	// Sin embargo parece que el simulador los ignora y no aparecen en la ventana de ocnfiguración de EXT Interrupts
	// configure EXTINT0 active if a rising-edge is detected
	//	EXTMODE 	=	1; //1 edge, 0 level
	//	EXTPOLAR	=	1; // 1 high, rising-edge; 0 low, falling-edge
	//  prueba = EXTMODE;
	
		// reset vars
	eint1_nueva_pulsacion = 0;
	eint1_cuenta = 0;
	
		// clear interrupt flag   
	EXTINT =  EXTINT | 2;          	
	
		// configuration of the IRQ slot number 3
		// of the VIC for EXTINT2
	VICVectAddr3 = (unsigned long)eint1_ISR;     
		
		// peripheral function EINT1 mapped to a related pin on pin connect block 
	  // (see table 56 of the LPC2105 user manual
		// set to pin P0.14
	PINSEL0 = PINSEL0 & 0xcfffffff;	//Sets bits 28 and 29 to 0
	PINSEL0 = PINSEL0 | 0x20000000;	// Enable the EXTINT1 interrupt
	
		// 0x20 bit 5 enables vectored IRQs. 
		// 15 is the number of the interrupt assigned. 
		// Number 15 is the eint2 (see table 40 of the LPC2105 user manual  
	VICVectCntl3 = 0x20 | 15;                   
	VICIntEnable = VICIntEnable | 0x00008000;  // write 1 on bit 15 

}




// EINT 2

void eint2_ISR (void) __irq 
{	
	VICIntEnClr = VICIntEnClr |  0x00010000; // disable eint2 interrupt
	EXTINT =  EXTINT | 4;   // clear interrupt flag        
	VICVectAddr = 0;        // Acknowledge Interrupt
	
	eint2_cuenta++;
	eint2_nueva_pulsacion = 1; 
	
		// call módulo gestión de ev_PULSACIONes 
	func_callback_eint2(2);
}


unsigned int eint2_leer_nueva_pulsacion_hal (void)
{ 
	return eint2_nueva_pulsacion; 
}


void eint2_clear_nueva_pulsacion_hal (void)
{ 
	eint2_nueva_pulsacion = 0; 
}


unsigned int eint2_leer_cuenta_hal (void)
{ 
	return eint2_cuenta; 
}

void eint2_reactivar_interrupciones_hal(void)
{
	// write 1 on EINT2 interrupt bit (14) to enable
	VICIntEnable = VICIntEnable | 0x00010000;
}

// Comprueba que eint2 sigue pulsado
unsigned int eint2_comprobar_flag(void)
{
	EXTINT = EXTINT | 4; // limpiar flag
	return EXTINT & 4 ? 1 : 0; // comprobar valor del flag
}
	
void eint2_iniciar_hal (void(*funcion_callback)(uint8_t) )
{
	// registrar función de callback para interrupción
	func_callback_eint2 = funcion_callback;
	
// NOTA: según el manual se puede configurar cómo se activan las interrupciones: por flanco o nivel, alta o baja. 
// Se usarían los registros EXTMODE y EXTPOLAR. 
// Sin embargo parece que el simulador los ignora y no aparecen en la ventana de ocnfiguración de EXT Interrupts
// configure EXTINT0 active if a rising-edge is detected
//	EXTMODE 	=	1; //1 edge, 0 level
//	EXTPOLAR	=	1; // 1 high, rising-edge; 0 low, falling-edge
//  prueba = EXTMODE;
	
		// reset vars
	eint2_nueva_pulsacion = 0;
	eint2_cuenta = 0;
	
		// clear interrupt flag 
	EXTINT =  EXTINT | 4;         
	
		// configuration of the IRQ slot number 4
		// of the VIC for EXTINT2
	VICVectAddr4 = (unsigned long)eint2_ISR;  // set interrupt vector in 4
    
		// peripheral function EINT2 mapped to a related pin on pin connect block 
	  // (see table 56 of the LPC2105 user manual(
		// set to pin P0.15
	PINSEL0 = PINSEL0 & 0x3fffffff;	//Sets bits 30 and 31 to 0
	PINSEL0 = PINSEL0 | 0x80000000;	// Enable the EXTINT2 interrupt
	
		// 0x20 bit 5 enables vectored IRQs. 
		// 16 is the number of the interrupt assigned. 
		// Number 16 is the eint2 (see table 40 of the LPC2105 user manual  
	VICVectCntl4 = 0x20 | 16;                   
  VICIntEnable = VICIntEnable | 0x00010000; 
}



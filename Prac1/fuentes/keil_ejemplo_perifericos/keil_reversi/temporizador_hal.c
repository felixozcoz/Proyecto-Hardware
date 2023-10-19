#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include <LPC210X.H> // LPC21XX Peripheral Registers
#include "temporizador_hal.h"
#include "pulsacion.h"

// Constante que permite convertir de ticjs a microsegundos
// const uint_16 temporizador_hal_ticks2us = ...  // el tipo lo he puesto por la cara

// variable para contabilizar el n�mero de interrupciones
static volatile unsigned int countTimer0 = 0;

// Declaraci�n de funciones gestoras de interrupciones
void timer_ISR (void) __irq;    // Generate Interrupt 

// ----------- GESTI�N DE INTERRUPCIONES -----------

/* Timer Counter 0 Interrupt executes each 10ms @ 60 MHz CPU Clock */
void timer_ISR (void) __irq {
			// incrementamos contador de interrupciones de timer 0
    countTimer0++;
			// limpiar flag de interrupci�n
    T0IR = 1; // registro 32-bits, escribir 1 l�gico en bit en el correspondiente
			// escribir 0 l�gico en este registro 
			// para limpiar el flag de interrupci�n del VIC
		VICVectAddr = 0;                             
}



// -------- GESTI�N DE VARIABLES CONTADOR -------------

unsigned int temporizador_hal_read_count(void){
	return countTimer0;
};

// ----------- TEMPORIZADOR DE TIMER 0 ----------------

// Funci�n que programa un contador para que pueda ser utilizado
void temporizador_hal_iniciar(void) 
{
		// inicializar contador de timer 0
		countTimer0= 0;	
	
		// Configuraci�n de Timer 0
	
		// Interrumpe cada 0,05ms = 150.000-1 counts @ 60 MHz ??
		T0MR0 = (60000 * 0.05) -1;  // registro 32-bits, Interrupt Register
	
		// Genera una interrupci�n (bit 0)
		// 	y reinicia TC si T0TC == T0MR0 (bit 1)
    T0MCR = 3;	// Match Control Register

		// Habilitar registro del Timer 0
    T0TCR = 1;	// Timer Control Register	
	
		// Configuraci�n del IRQ slot number 0 of 
		// 	the VIC for Timer 0 Interrupt
	
		// Se especifica la @ de la rutina de manejo 
		// de las interrupciones para el slot 0 del VIC
		VICVectAddr0 = (unsigned long)timer_ISR; 
    // 0x20 bit 5 enables vectored IRQs. 
		// 4 is the number of the interrupt assigned. Number 4 is the Timer 0 (see table 40 of the LPC2105 user manual  
		VICVectCntl0 = 0x20 | 4;                   
    VICIntEnable = VICIntEnable | 0x00000010;
}

// Inicia la cuenta del contador de forma indefinida
void temporizador0_hal_empezar(void) 
{
    countTimer0 = 0;
		T0TCR = 1;  // Habilita contador Timer 0 (TC) y PC
}

// Funci�n que lee el tiempo que lleva 
// contando el contador desde la �ltima vez que se ejecut� 
// temporizador0_hal_empezar  y lo devuelve en ticks.
uint64_t temporizador0_hal_leer(void) {
    return countTimer0;
}

// Funci�n para detener el contador y devolver 
// 	el tiempo transcurrido en ticks
// 	desde el �ltimo temporizador_hal_empezar.
uint64_t temporizador0_hal_parar() {
	T0TCR = 3; // habilitar interrupci�n y reinicio, reinicio en siguiente PCLK
	T0TCR = 1; // deshabilitar reinicializaci�n
	
	countTimer0 = 0; 	// reiniciar contador
	// qu� debe devolver???
}

#include <stdio.h>
#include <LPC210X.H> // LPC21XX Peripheral Registers
#include <inttypes.h>

#include "temporizador_hal.h"

// Constante que permite convertir de ticks a microsegundos
// con una frecuencia de procesador de 60MHz y VPBDIV = 4 (por defecto)
// algunos perif�ricos utilizan el PCLK que es frecuencia del procesador / VPBDIV
const float temporizador_hal_ticks2us =  1.0f/(15000.0f);

// variable para contabilizar el n�mero de interrupciones
static volatile unsigned int countTimer0 = 0;
static volatile unsigned int countTimer1 = 0;

// funci�n de callback
void (*funcion_callback)(void) = NULL;

// periodo
static unsigned int periodo = 0;

// ----------- GESTI�N DE INTERRUPCIONES -----------

// Funci�n gestora de interrupciones del temporizador (privada)
void timer0_ISR (void) __irq;    

void timer0_ISR (void) __irq {
			// incrementamos contador de interrupciones de timer 0
    countTimer0++;
			// limpiar flag de interrupci�n
    T0IR = 1; // registro 32-bits, escribir 1 l�gico en bit en el correspondiente
			// escribir 0 l�gico en este registro 
			// para limpiar el flag de interrupci�n del VIC
		VICVectAddr = 0;                             
}

// Funci�n gestora de interrupciones del temporizador (privada)
void timer1_ISR (void) __irq;    

void timer1_ISR (void) __irq {
			// funci�n del driver que ejecuta funci�n de callback
		funcion_callback();
			// incrementamos contador de interrupciones de timer 0
    countTimer1++;
			// limpiar flag de interrupci�n
    T1IR = 1; // registro 32-bits, escribir 1 l�gico en bit en el correspondiente
			// escribir 0 l�gico en este registro 
			// para limpiar el flag de interrupci�n del VIC
		VICVectAddr = 0;                             
}

// ----------- API HAL ----------------

// TIMER 0

// Funci�n que programa un contador para que pueda ser utilizado
void temporizador0_hal_iniciar(void) 
{
	
		// Configuraci�n de Timer 0
	
		// Timer0 como alg�n perif�rico va a una velocidad de 60MHz/VPBDIV ,
		// donde VPBDIV = 4 por defecto, entonces va a 15MHz
		T0MR0 = 14999;  // una interrupci�n cada 1ms
	
	
		// Genera una interrupci�n (bit 0)
		// 	y reinicia TC si T0TC == T0MR0 (bit 1)
    T0MCR = 3;	// Match Control Register

		// Deshabilitar/Detener registro del Timer 0 despu�s de reinicio
    T0TCR = 0;	// Timer Control Register	
	
		// Configuraci�n del IRQ slot number 0 of 
		// 	the VIC for Timer 0 Interrupt
	
		// Se especifica la @ de la rutina de manejo 
		// de las interrupciones para el slot 0 del VIC
		VICVectAddr0 = (unsigned long)timer0_ISR; 
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

// Lee el tiempo que lleva contando 
// el contador desde la �ltima vez que se ejecut� 
// temporizador_hal_empezar  y lo devuelve en ticks.
uint64_t temporizador0_hal_leer(void) {
    return countTimer0;
}

// Funci�n para detener el contador y devolver 
// 	el tiempo transcurrido en ticks
// 	desde el �ltimo temporizador_hal_empezar.
uint64_t temporizador0_hal_parar(void) {
	T0TCR = 3; // habilitar interrupci�n y reinicio, reinicio en siguiente PCLK
	T0TCR = 1; // deshabilitar reinicializaci�n
	
	return countTimer0;
}


// TIMER 1

// Funci�n que programa el reloj para que llame a la funci�n de 
// callback cada periodo. El periodo se indica en ms.
// Si el periodo es 0 se para el temporizador
void temporizador_hal_reloj (uint32_t _periodo, void (*_funcion_callback)())
{

		periodo = _periodo;
		// Inicializar contador con periodo en ticks
		T1MR0 = periodo / temporizador_hal_ticks2us;
		
		// Genera una interrupci�n (bit 0)
		// 	y reinicia TC si T1TC == T0MR1 (bit 1)
    T1MCR = 3;	// Match Control Register

		// Deshabilitar/Detener registro del Timer 1 despu�s de reinicio
    T1TCR = 1;	// Timer Control Register	
	
		// Configuraci�n del IRQ slot number 1 of 
		// 	the VIC for Timer 1 Interrupt
	
		// guardamos funci�n de callback cuando interrupci�n
		funcion_callback = _funcion_callback;
	
		// Se especifica la @ de la rutina de manejo 
		// de las interrupciones para el slot 0 del VIC
		VICVectAddr1 = (unsigned long)timer1_ISR; 
    // 0x20 bit 5 enables vectored IRQs. 
		// 5 is the number of the interrupt assigned. Number 5 is the Timer 0 (see table 40 of the LPC2105 user manual  
		VICVectCntl1 = 0x20 | 5;                   
    VICIntEnable = VICIntEnable | 0x00000020;
}


// Lee el tiempo que lleva contando 
// el contador desde la �ltima vez que se ejecut� 
// temporizador_hal_empezar  y lo devuelve en ticks.
uint64_t temporizador1_hal_leer(void) {
    return countTimer1;
}

// Funci�n para detener el contador y devolver 
// 	el tiempo transcurrido en ticks
// 	desde el �ltimo temporizador_hal_empezar.
uint64_t temporizador1_hal_parar(void) {
	T1TCR = 3; // habilitar interrupci�n y reinicio, reinicio en siguiente PCLK
	T1TCR = 0; // deshabilitar reinicializaci�n
	
	return countTimer1;
}

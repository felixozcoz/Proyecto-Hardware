#include "linea_serie_hal.h"

void UART0_ISR (void) __irq {
	// ...
		VICVectAddr = 0;   // Acknowledge interrupt                    
}


void init_serial(void)
{
	// Seleccionar los pines de UART0 en los registros 
	PINSEL0 = PINSEL0 | 0x5;  
	
	// Configurar Baud rate (velocidad de transmisión)
	// DLL y DLM accesible sólo si DLAB=1 (the Divisor Latch Access)
	U0LCR = 0x83;		// 8 bits, no Parity, 1 Stop bit    
	U0DLL = 97;			// 9600 baud rate @ 15MHz VPB (Vectored Peripheral Bus) Clock
	U0LCR = 0x3;	  // disable DLAB
	
	// Habilita las interrupciones de los registros de
	// lectura (U0RBR), de escritura (U0THR) y lectura 
	// la fifo de entrada (RX FIFO) ( See table 84 of the LPC2105 user manual )
	U0IER = U0IER | 0x7;
	
	// Configurar VIC
	VICVectAddr5 = (unsigned long)UART0_ISR;
	// enable vectored IRQ's and 6 is the number of the interrupt assigned. 
	VICVectCntl5 = 0x20 | 6; 		
	VICIntEnable = VICIntEnable | 0x40;
}

void sendchar(int ch)
{	
	// Transmit Holding Register; Transmite los datos a la UART
	U0THR = ch;	
	
	// TODO: se debería chequear que el dato es válido usando el U0LSR?
}

int getchar(void)
{
	// UART0 Line Status Register: read-only , provides status info on the RX and TX
	// (see table 89 of the LPC2105 user manual) 
	while( ! (U0LSR & 0x01) ); // check U0RBR contains valid data on U0RBR
	
	return U0RBR;
	
}
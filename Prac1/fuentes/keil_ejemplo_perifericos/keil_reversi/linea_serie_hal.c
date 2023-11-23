#include "linea_serie_hal.h"

// función de callback del gestor
static void(*funcion_callback_transmision)(void) = NULL;
static void(*funcion_callback_recepcion)(void) = NULL;


void UART0_ISR (void) __irq {
	// indentificar origen de la interrupción
	switch(U0IIR & 0xF){ 
		case 0x2 : // transmisión completada
			funcion_callback_transmision();
		break;
			
		case 0x4 : // recepción de un caracter
			funcion_callback_recepcion();
		break;
	}
	
	VICVectAddr = 0;   // Acknowledge interrupt                    
}


void iniciar_serial_hal( void(*_funcion_callback_transmision)(), void(*_funcion_callback_recepcion)() )
{
	// registrar funciones de callback para interrupciones
	funcion_callback_transmision = _funcion_callback_transmision; 
	funcion_callback_recepcion = _funcion_callback_recepcion;
	
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

void sendchar_serie(char ch)
{	
	// Transmit Holding Register; Transmite los datos a la UART
	U0THR = ch;	
}

int getchar_serie(void)
{
	// UART0 Line Status Register: read-only , provides status info on the RX and TX
	// (see table 89 of the LPC2105 user manual) 
	// TODO: GESTIONAR, EN VEZ DE ESPERA ACTIVA
	while(! (U0LSR & 0x01) ); // check contains valid data
	
	return U0RBR;
}

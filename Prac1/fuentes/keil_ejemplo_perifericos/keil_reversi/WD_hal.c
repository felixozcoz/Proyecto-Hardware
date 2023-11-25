#include "WD_hal.h"

#include <LPC210x.h>
#include "SWI_hal.h"

// WD_hal_inicializar
//
// Parámetros:
// 	sec: tiempo en s a asignar 
//		al tiempo de cuenta del WD
//
// Asigna un tiempo igual a 'sec'
// al tiempo de cuenta del WD (en us). Cambia
// el modo del WD a 'Reset Mode' (Table 188. LPC210x User Manual Guide)
// See page 221 LPC210x User Manual Guide: 
// 	"The watchdog consists of a divide by 4 fixed pre-scaler and a 32-bit counter"
void WD_hal_inicializar(uint32_t sec)
{
	WDMOD &= ~0x04;									// Se pone el bit 3 WDINT del WD a 0 para resetear el flag de timeout
	WDTC =(15000 * 1000 * sec)/4;		// Set timeout a sec (en us) 
	WDMOD = 0x03;										// Set 'Reset Mode' WD mode (WDEN = 1, WDRESET = 1)
}

// WD_hal_feed
//
// "Alimentar" Watchdog. Reinicia
// la cuenta del WD si no ha terminado.
void WD_hal_feed(void)
{
	bit_irq = read_IRQ_bit(); 
	
	if ( bit_irq )
		disable_irq();
	
	// escrituras consecutivas para "alimentar" al WD
	WDFEED = 0xAA;						   
  WDFEED = 0x55; 
	
	if ( bit_irq )
		enable_irq(); // restaurar el estado del bit I (disable IRQ interrupt)
}


void WD_hal_test(uint32_t sec){
  WD_hal_inicializar(sec);
	WD_hal_feed();
	
	while(1);
}

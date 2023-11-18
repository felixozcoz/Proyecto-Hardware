#include "WD_hal.h"



void WD_hal_inicializar(uint32_t sec){
	WDMOD &= ~0x04;						// Se pone el bit 3 WDINT del WD a 0 para resetear el flag de timeout
	
	WDTC =15000 * 1000 * sec;							// Se pone el timeout del WD a 1us 
	
	WDMOD = 0X03;								// Se resetea el WD
}

void WD_hal_feed(){
	int bit_irq;
	
	bit_irq = read_IRQ_bit();
	disable_fiq();
	
	if (bit_irq == 1){
		disable_irq();
	}
	
	WDFEED = 0xAA;						   
  WDFEED = 0x55; 
	
	if (bit_irq == 1){
		enable_irq();
	}
}


void WD_hal_test(){
	WD_hal_inicializar();
	WD_hal_feed();
	while(1){
	
	}
}

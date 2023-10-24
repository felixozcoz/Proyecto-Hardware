#include "power.h"
#include "Power_management.h"
#include <LPC210x.H>  

void power_hal_wait(void){
	EXTWAKE = 1; // EXTINT0 will awake the processor
	PCON |= 0x01; 
	Switch_to_PLL();
}

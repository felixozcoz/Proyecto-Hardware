#include "power.h"
#include "Power_management.h"
#include <LPC210x.H>  

// Set the processor into idle state 
// so any peripheral can wake it up
void power_hal_wait(void){
	EXTWAKE = 1; // EXTINT0 will awake the processor
	PCON |= 0x01; // idle mode
	Switch_to_PLL();
}

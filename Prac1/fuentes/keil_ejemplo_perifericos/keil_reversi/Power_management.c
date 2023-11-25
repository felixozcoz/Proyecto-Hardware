#include <LPC210x.H>      
#include "Power_management.h"
#include "power_hal.h"

// Set the processor into idle mode
// so any peripheral can wake up the processor
void PM_power_down (void)  {
  power_hal_deep_sleep();
}


void PM_idle(void){
	power_hal_wait();
}

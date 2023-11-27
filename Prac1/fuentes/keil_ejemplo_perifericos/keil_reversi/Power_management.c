#include <LPC210x.H>      
#include "Power_management.h"
#include "power_hal.h"

// Cambia el modo de estado del 
// procesador a DEEP-SLEEP (suspensión profunda)
void PM_power_down (void)  {
  power_hal_deep_sleep();
}

// Cambia el modo de estado del procesador a IDLE (reposo)
void PM_idle(void){
	power_hal_wait();
}

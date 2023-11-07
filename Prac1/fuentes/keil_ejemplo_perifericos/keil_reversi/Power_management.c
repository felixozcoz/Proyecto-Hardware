#include <LPC210x.H>      
#include "Power_management.h"

// Set the processor into idle mode
// so any peripheral can wake up the processor
void PM_power_down (void)  {
  power_hal_wait();
}

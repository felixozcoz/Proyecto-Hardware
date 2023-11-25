#include "power_hal.h"
#include "Power_management.h"
#include <LPC210x.H>  

extern void Switch_to_PLL(void);

// Configurar procesador a 
// estado IDLE. Puede ser 
// despertado por cualquier periférico
void power_hal_wait(void){
	PCON |= 0x01; // idle mode
	Switch_to_PLL();
}

// Configurar el procesador
// a estado power-down. Únicamente
// puede ser despertado por los
// periféricos indicados en EXTWAKE
void power_hal_deep_sleep(void)
{
	EXTWAKE = 6; // EXTINT1 and EXTINT2 will awake the processor
	PCON |= 0x02; // power-down mode
	Switch_to_PLL();
}

#include "power.h"
#include "Power_management.h"
#include <LPC210x.H>  

// Configurar procesador a 
// estado IDLE. Puede ser 
// despertado por cualquier perif�rico
void power_hal_wait(void){
	PCON |= 0x01; // idle mode
	Switch_to_PLL();
}

// Configurar el procesador
// a estado power-down. �nicamente
// puede ser despertado por los
// perif�ricos indicados en EXTWAKE
void power_hal_deep_sleep(void)
{
	EXTWAKE = 6; // EXTINT1 and EXTINT2 will awake the processor
	PCON |= 0x02; // power-down mode
	Switch_to_PLL();
}
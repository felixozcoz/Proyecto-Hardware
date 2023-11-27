#include "power_hal.h"
#include "Power_management.h"
#include <LPC210x.H>  

// Declarada en Startup.s, permite 
// volver al procesador al modo de 
// operaci�n normal del procesador
extern void Switch_to_PLL(void);


// Cambia el modo de estado del procesador a IDLE (reposo)
void power_hal_wait(void){
	PCON |= 0x01; // idle mode
	Switch_to_PLL();	// volver a estado de operaci�n normal
}

// Cambia el modo de estado del 
// procesador a DEEP-SLEEP (suspensi�n profunda)
// �nicamente puede ser despertado por los
// perif�ricos indicados en EXTWAKE
void power_hal_deep_sleep(void)
{
	EXTWAKE = 6; // EXTINT1 and EXTINT2 will awake the processor
	PCON |= 0x02; // power-down mode
	Switch_to_PLL(); // volver a estado de operaci�n normal
}

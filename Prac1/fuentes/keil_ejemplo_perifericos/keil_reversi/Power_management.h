#ifndef	POWER_MANAGEMENT_H
#define POWER_MANAGEMENT_H

// Cambia el modo de estado del 
// procesador a DEEP-SLEEP (suspensión profunda)
// Determinadas interrupciones lo despiertan (configurable)
void PM_power_down (void); 

// Cambia el modo de estado del procesador a IDLE (reposo)
// Cualquier interrupción lo despierta
void PM_idle(void);


#endif // POWER_MANAGEMENT_H

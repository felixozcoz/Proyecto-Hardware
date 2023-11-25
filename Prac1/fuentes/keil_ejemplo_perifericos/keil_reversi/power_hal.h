#ifndef POWER_HAL_H
#define POWER_HAL_H

// El procesador pasa al estado IDLE
// cualquier interrupci�n de un 
// perif�rico despierta al procesador
void power_hal_wait(void);

// El procesador pasa al estado POWER-DOWN
void power_hal_deep_sleep(void);


#endif // POWER_HAL_H

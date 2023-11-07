#ifndef POWER_H
#define POWER_H

// El procesador pasa al estado IDLE
// cualquier interrupción de un 
// periférico despierta al procesador
void power_hal_wait(void);


// El procesador pasa al estado POWER-DOWN
void power_hal_deep_sleep(void);


#endif // POWER_H

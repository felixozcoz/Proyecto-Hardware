#ifndef _LINEA_SERIE_DRV_H_
#define _LINEA_SERIE_DRV_H_

// M�dulo interface de perif�rico UART0

#include "GPIO_hal.h"
#include "Mensaje_t.h"

// Inicializa el perif�rico UART0
// e inicializa los pines de GPIO utilizados
void iniciar_serial(const GPIO_HAL_PIN_T _pin, const GPIO_HAL_PIN_T _num_pins);

// Funci�n de lectura de l�nea serie.
// Su comportamiento est� definido por una fsm
// Mealy cuya entrada son los caracteres le�dos
// de la l�nea serie
void gestor_serial(void);

// Funci�n de escritura de l�nea serie.
// buff apunta a una secuencia de caracteres
// que eventualmente ser�n escritos en la UART0. 
void linea_serie_drv_enviar_array(Mensaje_t msg);


#endif // _LINEA_SERIE_DRV_H_

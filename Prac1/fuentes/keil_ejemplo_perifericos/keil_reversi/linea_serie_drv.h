#ifndef _LINEA_SERIE_DRV_H_
#define _LINEA_SERIE_DRV_H_

// Módulo interface de periférico UART0

#include "GPIO_hal.h"
#include "Mensaje_t.h"

// Inicializa el periférico UART0
// e inicializa los pines de GPIO utilizados
void iniciar_serial(const GPIO_HAL_PIN_T _pin, const GPIO_HAL_PIN_T _num_pins);

// Función de lectura de línea serie.
// Su comportamiento está definido por una fsm
// Mealy cuya entrada son los caracteres leídos
// de la línea serie
void gestor_serial(void);

// Función de escritura de línea serie.
// buff apunta a una secuencia de caracteres
// que eventualmente serán escritos en la UART0. 
void linea_serie_drv_enviar_array(Mensaje_t msg);


#endif // _LINEA_SERIE_DRV_H_

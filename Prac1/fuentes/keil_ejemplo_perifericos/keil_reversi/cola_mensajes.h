#ifndef _COLA_MENSAJES_H_
#define _COLA_MENSAJES_H_

#include <stdbool.h>
#include "Mensaje_t.h"
#include "GPIO_hal.h"


#define MAX_CAPACIDAD_MSG_FIFO 32

// Definición de la cola de mensajes
typedef struct {
    Mensaje_t elementos[MAX_CAPACIDAD_MSG_FIFO];
    int frente, fin;
    int capacidad;
} ColaMensajes;

// Funciones para la cola de mensajes
void inicializar_cola_mensajes(const GPIO_HAL_PIN_T _pin_overflow);

bool estaVacia_msg(void);

bool estaLlena_msg(void);

bool encolar_msg(Mensaje_t mensaje);

bool desencolar_msg(Mensaje_t *Mensaje);


#endif // _COLA_MENSAJES_H_

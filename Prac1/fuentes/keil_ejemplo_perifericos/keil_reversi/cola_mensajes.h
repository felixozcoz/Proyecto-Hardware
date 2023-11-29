#ifndef _COLA_MENSAJES_H_
#define _COLA_MENSAJES_H_

#include <stdio.h>
#include <stdbool.h>
#include "Mensaje_t.h"


#define MAX_CAPACIDAD_MSG_FIFO 32

// Definición de la cola de mensajes
typedef struct {
    Mensaje_t elementos[MAX_CAPACIDAD_MSG_FIFO];
    int frente, fin;
    int capacidad;
} ColaMensajes;

// Funciones para la cola de mensajes
void inicializar_cola_mensajes(ColaMensajes *cola);

bool estaVacia(ColaMensajes *cola);

bool estaLlena(ColaMensajes *cola);

bool encolar(ColaMensajes *cola, const char *contenido);

bool desencolar(ColaMensajes *cola, Mensaje_t *msg);

	
	




#endif // _COLA_MENSAJES_H_

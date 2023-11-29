#include <stdio.h>
#include <string.h>
#include "cola_mensajes.h"


// Funciones para la cola de mensajes
void inicializar_cola_mensajes(ColaMensajes *cola) {
    cola->frente = cola->fin = -1;
		cola->capacidad = MAX_CAPACIDAD_MSG_FIFO;
}

bool estaVacia(ColaMensajes *cola) {
    return cola->frente == -1;
}

bool estaLlena(ColaMensajes *cola) {
    return (cola->fin + 1) % cola->capacidad == cola->frente;
}

bool encolar(ColaMensajes *cola, const char *contenido) {
		// quedan mensajes sin procesar	
		if (estaLlena(cola)) 
        return 0;
		
		// mensaje demasiado grande
		if(strlen(contenido) > MAX_MENSAJE_LENGTH) 
			return 0;

    if (estaVacia(cola)) 
        cola->frente = cola->fin = 0;
    else
        cola->fin = (cola->fin + 1) % cola->capacidad;
    
    strncpy(cola->elementos[cola->fin], contenido, strlen(contenido)+1);
		return 1;
}

bool desencolar(ColaMensajes *cola, Mensaje_t *msg) {
    if (estaVacia(cola))
        return 0;
		
		// almacenar mensaje
		strncpy(cola->elementos[cola->frente], *msg, strlen(cola->elementos[cola->frente]));

    if (cola->frente == cola->fin) {
        cola->frente = cola->fin = -1;
    } else {
        cola->frente = (cola->frente + 1) % cola->capacidad;
    }
		
		return 1;
}

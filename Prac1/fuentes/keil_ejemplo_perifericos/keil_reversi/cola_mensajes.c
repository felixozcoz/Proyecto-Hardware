#include <stdio.h>
#include <string.h>
#include "cola_mensajes.h"
#include "SWI_hal.h"


// Pins GPIO (solo un pin)
static GPIO_HAL_PIN_T pin_overflow;
// Cola de mensajes
static ColaMensajes cola;

// Funciones para la cola de mensajes
void inicializar_cola_mensajes(const GPIO_HAL_PIN_T _pin_overflow) {
			// set realacionado con GPIO
		pin_overflow = _pin_overflow;
		gpio_hal_sentido(pin_overflow,1,GPIO_HAL_PIN_DIR_OUTPUT);
			// init cola
		cola.frente = cola.fin = -1;
		cola.capacidad = MAX_CAPACIDAD_MSG_FIFO;
}

bool estaVacia(void) {
    return (cola.frente == -1);
}

bool estaLlena(void) {
    return ((cola.fin + 1) % cola.capacidad == cola.frente);
}

bool encolar(const char *contenido) {
			// deshabilitar interrupciones para atomicidad
		bit_irq = read_IRQ_bit(); 
		if ( bit_irq )
			disable_irq();
		
			// quedan mensajes sin procesar	
		if ( estaLlena() ) {
				// overflow de mensajes
			gpio_hal_escribir(pin_overflow, 1, 1);
				// restaurar estado de irq
			if ( bit_irq )
				enable_irq();
			
			while(1);
		}
		
		// mensaje demasiado grande
		if( strlen(contenido) > MAX_MENSAJE_LENGTH ){
			if ( bit_irq )
				enable_irq();
			return 0;
		}

    if( estaVacia() ) 
        cola.frente = cola.fin = 0;
    else
        cola.fin = (cola.fin + 1) % cola.capacidad;
    
    strncpy(cola.elementos[cola.fin], contenido, strlen(contenido)+1);
			// restaurar interrupciones irq
		if ( bit_irq )
			disable_irq();
		
		return 1;
}

bool desencolar(Mensaje_t *msg) {
				// deshabilitar interrupciones para atomicidad
		bit_irq = read_IRQ_bit(); 
		if ( bit_irq )
			disable_irq();
		
    if (estaVacia()){
					// restaurar interrupciones irq
				if ( bit_irq )
					disable_irq();
        return 0;
		}
		// almacenar mensaje
		strncpy(*msg, cola.elementos[cola.frente], strlen(cola.elementos[cola.frente])+1);

    if (cola.frente == cola.fin) {
        cola.frente = cola.fin = -1;
    } else {
        cola.frente = (cola.frente + 1) % cola.capacidad;
    }
		
			// restaurar interrupciones irq
		if ( bit_irq )
			disable_irq();
		
		return 1;
}

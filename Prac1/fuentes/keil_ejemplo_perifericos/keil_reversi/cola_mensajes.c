#include <stdio.h>
#include <string.h>
#include "cola_mensajes.h"
#include "SWI_hal.h"

// Pins GPIO (solo un pin)
static GPIO_HAL_PIN_T pin_overflow;
// Cola de mensajes

static Mensaje_t elementos[MAX_CAPACIDAD_MSG_FIFO];
static volatile int frente, fin;
static volatile int capacidad;

// Funciones para la cola de mensajes
void inicializar_cola_mensajes(const GPIO_HAL_PIN_T _pin_overflow) {
			// set realacionado con GPIO
		pin_overflow = _pin_overflow;
		gpio_hal_sentido(pin_overflow,1,GPIO_HAL_PIN_DIR_OUTPUT);
			// init cola
		frente = fin = -1;
		capacidad = MAX_CAPACIDAD_MSG_FIFO;
}

bool estaVacia_msg(void) {
    return (frente == -1);
}

bool estaLlena_msg(void) {
    return ((fin + 1) % capacidad == frente);
}

bool encolar_msg(Mensaje_t mensaje) {
			// deshabilitar interrupciones para atomicidad
		bit_irq = read_IRQ_bit(); 
		if ( bit_irq )
			disable_irq();
		
			// quedan mensajes sin procesar	
		if ( estaLlena_msg() ) {
				// overflow de mensajes
			gpio_hal_escribir(pin_overflow, 1, 1);
				// restaurar estado de irq
			if ( bit_irq )
				enable_irq();
			
			while(1);
		}
		
		// mensaje demasiado grande
		if( strlen(mensaje) > MAX_MENSAJE_LENGTH ){
			if ( bit_irq )
				enable_irq();
			return 0;
		}

    if( estaVacia_msg() ) 
        frente = fin = 0;
    else
        fin = (fin + 1) % capacidad;
    
		setMensaje(elementos[fin], mensaje);
			// restaurar interrupciones irq
		if ( bit_irq )
			enable_irq();
		
		return 1;
}

bool desencolar_msg(Mensaje_t mensaje) {
				// deshabilitar interrupciones para atomicidad
		bit_irq = read_IRQ_bit(); 
		if ( bit_irq )
			disable_irq();
		
    if (estaVacia_msg()){
					// restaurar interrupciones irq
				if ( bit_irq )
					enable_irq();
        return 0;
		}
		// almacenar mensaje
		setMensaje(mensaje, elementos[frente]);

    if (frente == fin) {
        frente = fin = -1;
    } else {
        frente = (frente + 1) % capacidad;
    }
		
			// restaurar interrupciones irq
		if ( bit_irq )
			enable_irq();
		
		return 1;
}

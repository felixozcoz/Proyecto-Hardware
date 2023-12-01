#include "cola_fifo.h"
#include "SWI_hal.h"


// Elementos de la cola
typedef struct {
    EVENTO_T ID_evento;
    uint32_t auxData;	// informaci�n extra del evento
} FIFO_ELEMENTO_T;	

// Declaraci�n de la cola mo un arreglo de elementos
static FIFO_ELEMENTO_T fifo[MAX_SIZE_FIFO];

// Variables para control de la cola
static volatile uint8_t fifo_inicio = 0; // �ndice de cola fifo del �ltimo evento no tratado
static volatile uint8_t fifo_fin = 0; // �ndice de cola fifo donde se debe a�adidir el siguiente evento

// Pins GPIO (un solo pin)
static GPIO_HAL_PIN_T pin_overflow; 

// Variable que guarda el n�mero de veces
// que el evento i se ha encolado
static uint32_t estadisticas[MAX_SIZE_FIFO] = {0};


void FIFO_inicializar(const GPIO_HAL_PIN_T _pin_overflow) {
		fifo_fin = 0;
		fifo_inicio = 0;
		pin_overflow = _pin_overflow;
		// La cola est� llena, manejar el desbordamiento/overflow
		gpio_hal_sentido(pin_overflow, 1, GPIO_HAL_PIN_DIR_OUTPUT);
}

void FIFO_encolar( EVENTO_T ID_evento, uint32_t auxData) {
	
		// deshabilitar interrupciones para atomicidad
		bit_irq = read_IRQ_bit(); 
		if ( bit_irq )
			disable_irq();
		
    // Verifica si la cola est� llena antes de encolar un evento
     if ((fifo_fin + 1) % MAX_SIZE_FIFO == fifo_inicio) {
				// set pin overflow encendido
				gpio_hal_escribir(pin_overflow, 1, 1);
			 
				// restaurar el estado del bit I (disable IRQ interrupt)
				if ( bit_irq )
					enable_irq();
			 
				while(1);	// terminar ejecuci�n
			
    } else {
				// Crear elemento para la cola fifo
				fifo[fifo_fin].ID_evento = ID_evento;
        fifo[fifo_fin].auxData = auxData;
			
        fifo_fin = (fifo_fin + 1) % MAX_SIZE_FIFO;
			
        estadisticas[ID_evento]++;	// contabiliza evento
    }
		
		// restaurar el estado del bit I (disable IRQ interrupt)
		if ( bit_irq )
			enable_irq();
}


// Si hay eventos sin procesar devuelve
// la referencia al evento m�s antiguo sin procesar.
// En cualquier otro caso, devuelve 0, que indica que
// la cola est� vac�a
uint8_t FIFO_extraer(EVENTO_T *ID_evento, uint32_t *auxData) {
		uint8_t retVal = 0;
	
		// deshabilitar interrupciones para atomicidad
		bit_irq = read_IRQ_bit(); 
		if ( bit_irq )
			disable_irq();
	
    // Verifica si la cola est� vac�a
    if (fifo_inicio == fifo_fin) {
        // La cola est� vac�a
        retVal = 0;
    } else {
				// devuelve referencia del evento
        *ID_evento = fifo[fifo_inicio].ID_evento;
        *auxData = fifo[fifo_inicio].auxData;
        fifo_inicio = (fifo_inicio + 1) % MAX_SIZE_FIFO;
        retVal = 1;
    }
		
			// restaurar el estado del bit I (disable IRQ interrupt)
		if ( bit_irq )
			enable_irq(); 
		
		return retVal;
}


uint32_t FIFO_estadisticas(const EVENTO_T ID_evento) {
    if (ID_evento == ev_EVENTO_VOID) {
        // Si se solicitan estad�sticas para ev_EVENTO_VOID, se devuelve el total de eventos encolados
        return estadisticas[ev_EVENTO_VOID];
    } else {
        return estadisticas[ID_evento];
    }
}

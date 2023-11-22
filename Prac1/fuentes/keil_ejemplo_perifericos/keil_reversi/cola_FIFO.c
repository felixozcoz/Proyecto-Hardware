#include "cola_fifo.h"

// Estructura para los 
// elementos de la cola
typedef struct {
    EVENTO_T ID_evento;
    uint32_t auxData;	// información extra del evento
} FIFO_ELEMENTO_T;	

// Tamaño máximo de la cola
const static unsigned int MAX_SIZE = 32;

// Declaración de la cola mo un arreglo de elementos
static FIFO_ELEMENTO_T fifo[MAX_SIZE];

// Variables para control de la cola
static uint8_t fifo_inicio = 0; // índice de cola fifo del último evento no tratado
static uint8_t fifo_fin = 0; // índice de cola fifo donde se debe añadidir el siguiente evento
static GPIO_HAL_PIN_T pin_overflow; 

// Variable que guarda el número de veces
// que el evento i se ha encolado
static uint32_t estadisticas[MAX_SIZE] = {0};




void FIFO_inicializar(const GPIO_HAL_PIN_T _pin_overflow) {
		fifo_fin = 0;
		fifo_inicio = 0;
		pin_overflow = _pin_overflow;
}

void FIFO_encolar( EVENTO_T ID_evento, uint32_t auxData) {
    // Verifica si la cola está llena antes de encolar un evento
     if ((fifo_fin + 1) % MAX_SIZE == fifo_inicio) {
        // La cola está llena, manejar el desbordamiento/overflow
				gpio_hal_sentido(pin_overflow, GPIO_OVERFLOW_BITS, GPIO_HAL_PIN_DIR_OUTPUT);
				gpio_hal_escribir(pin_overflow, GPIO_OVERFLOW_BITS, 1);
				
				while(1);	// terminar ejecución
			
    } else {
				// Crear elemento para la cola fifo
				fifo[fifo_fin].ID_evento = ID_evento;
        fifo[fifo_fin].auxData = auxData;
			
        fifo_fin = (fifo_fin + 1) % MAX_SIZE;
			
        estadisticas[ID_evento]++;	// contabiliza evento
    }
}


// Si hay eventos sin procesar devuelve
// la referencia al evento más antiguo sin procesar.
// En cualquier otro caso, devuelve 0, que indica que
// la cola está vacía
uint8_t FIFO_extraer(EVENTO_T *ID_evento, uint32_t *auxData) {
		uint8_t retVal = 0;
	
		// deshabilitar interrupciones para atomicidad
		bit_irq = read_IRQ_bit(); 
		if ( bit_irq )
			disable_irq();
	
    // Verifica si la cola está vacía
    if (fifo_inicio == fifo_fin) {
        // La cola está vacía
        retVal = 0;
    } else {
				// devuelve referencia del evento
        *ID_evento = fifo[fifo_inicio].ID_evento;
        *auxData = fifo[fifo_inicio].auxData;
        fifo_inicio = (fifo_inicio + 1) % MAX_SIZE;
        retVal = 1;
    }
		
			// restaurar el estado del bit I (disable IRQ interrupt)
		if ( bit_irq )
			enable_irq(); 
		
		return retVal;
}

// Devuelve el número de total de veces
// que ese evento con identificador
// ID_EVENTO se ha encolado
// NOTA: el evento EVENTO_VOID devuelve el
// 	total de eventos encolados desde el inicio
uint32_t FIFO_estadisticas(const EVENTO_T ID_evento) {
    if (ID_evento == EVENTO_VOID) {
        // Si se solicitan estadísticas para EVENTO_VOID, se devuelve el total de eventos encolados
        return estadisticas[EVENTO_VOID];
    } else {
        return estadisticas[ID_evento];
    }
}

unsigned int get_size_FIFO(void){
	return MAX_SIZE;
}

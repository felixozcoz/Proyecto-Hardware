#include "planificador.h"
#include "cola_FIFO.h"
#include "hello_world.h"
#include "power.h"
#include "alarmas.h"
#include "botones.h"

// Activa el pin de overflow en la GPIO // DEBERÍA ESTAR EN GPIO??
void activar_overflow_gpio_pin(void);



// (Función auxiliar)
//
// Inicializa GPIO con GPIO31
// (overflow bit) a output dir
void GPIO_inicializar(void);



// Inicilizar cola de eventos
void inicializar_cola_eventos(const uint32_t periodo_timer1) {
	EVENTO_T evento;
	uint32_t auxData;
	FIFO_inicializar(GPIO_OVERFLOW);
	
		// inicializar módulos
	// hello_world_inicializar(GPIO_HELLO_WORLD, GPIO_HELLO_WORLD_BITS);
	GPIO_inicializar(); // inicializar módulo GPIO
	alarma_inicializar(); // inicializar módulo Alarmas
	
	while(1){
		
		if( FIFO_extraer(&evento, &auxData) ){
			// tratar dato
			switch(evento){
				case EVENTO_HELLO_WORLD:
					//hello_tick_tack();
					break;
				
				case REVISAR_ALARMAS:
					// comprueba si tiene que disparar alguna alarma
					alarma_tratar_evento(periodo_timer1);
					break;
				
				case ALARMAS_OVERFLOW:
					activar_overflow_gpio_pin();
					while(1); // fin de la ejecución
				
				case PULSACION:
					if ( auxData )
						eint1_gestionar_pulsacion(); // EINT1
					else 
						eint2_gestionar_pulsacion(); // EINT2	
					break;
					
				default:
					break;
			}
		} else{
			// detener procesador y reaunador si ocurre interrupción
			power_hal_wait();
		}
	}
}



void GPIO_inicializar(void)
{
	gpio_hal_iniciar();
		// set GPIO13 (overflow) to output dir
	gpio_hal_sentido(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS, GPIO_HAL_PIN_DIR_OUTPUT);
}


__inline void activar_overflow_gpio_pin(void) {
		// escribir en GPIO31
	gpio_hal_escribir(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS, 1);
	
}

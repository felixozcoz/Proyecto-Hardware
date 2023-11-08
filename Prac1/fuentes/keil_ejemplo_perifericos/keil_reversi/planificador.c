#include "planificador.h"
#include "cola_FIFO.h"
#include "hello_world.h"
#include "power.h"
#include "alarmas.h"
#include "botones.h"
#include "tests.h"


// Tiempo sin actividad de usuario considerado
// para pasar a estado power-down del procesador
// static const unsigned int USUARIO_AUSENTE = 12;

// Activa el pin de overflow en la GPIO // DEBERÍA ESTAR EN GPIO??
void activar_overflow_gpio_pin(void);

// (Función auxiliar)
//
// Inicializa GPIO con GPIO31
// (overflow bit) a output dir
void GPIO_inicializar(void);


// Inicilizar cola de eventos
void inicializar_cola_eventos(const uint32_t periodo_timer1) {
	EVENTO_T evento = EVENTO_VOID; 
	uint32_t auxData = 0;

	
		// inicializar módulos
	// hello_world_inicializar(GPIO_HELLO_WORLD, GPIO_HELLO_WORLD_BITS);
	GPIO_inicializar(); // inicializar módulo GPIO
	#if  ! TEST_ALARMAS
		FIFO_inicializar(GPIO_OVERFLOW);
		alarma_inicializar(); // inicializar módulo Alarmas
	#endif 
	
	// programa alarma de inactividad (paso a power-down)
	// alarma_activar(POWER_DOWN, USUARIO_AUSENTE, 0);
	
	while(1){
		
			FIFO_extraer(&evento, &auxData);
		
			// tratar dato
			switch(evento){
				case EVENTO_HELLO_WORLD:
					//hello_tick_tack();
					break;
				
				case REVISAR_ALARMAS:
					// comprueba si tiene que disparar alguna alarma
					alarma_tratar_evento(periodo_timer1);
					evento = EVENTO_VOID; // reset evento
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
					
				case POWER_DOWN:
					// pasar a estado power-down
					power_hal_deep_sleep();
					break;
				
				default: 
					// para tratar eventos de alarmas
					#if TEST_ALARMAS
						if( evento < 0) break;
					#endif
				
					// alarma para pasar a power-down en caso de inactividad
					// paso a estado IDLE del procesador
					power_hal_wait();
					break;
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

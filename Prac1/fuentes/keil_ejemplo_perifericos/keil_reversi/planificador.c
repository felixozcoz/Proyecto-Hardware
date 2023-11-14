#include "planificador.h"

// Tiempo sin actividad de usuario considerado
// para pasar a estado power-down del procesador
static unsigned int USUARIO_AUSENTE = 1000; // en ms //12000; // en ms

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
	
	#if  ! ( TEST_ALARMAS | TEST_BOTONES )
		FIFO_inicializar(GPIO_OVERFLOW);
		alarma_inicializar(); // inicializar módulo Alarmas
			// programa alarma de inactividad (paso a power-down)
	#endif 
	
	#if DEMOSTRADOR
		inicializar_juego(0, 0);
	#endif
	
		// programa alarma para reducción de consumo a power-down state
	alarma_activar(POWER_DOWN, USUARIO_AUSENTE, 0);
	

		// programa dos alarmas, una para cada botón
	inicializar_botones(); 
	
	while(1){
			evento = EVENTO_VOID; // reset evento
			FIFO_extraer(&evento, &auxData);
		
			// tratar dato
			switch(evento){
				case ev_VISUALIZAR_HELLO:
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
					#if DEMOSTRADOR
						juego_tratar_evento(ev_VISUALIZAR_CUENTA, auxData);
					#endif
				
						// resetear alarma de power-down
					alarma_activar(POWER_DOWN, USUARIO_AUSENTE, 0);
				
						// handle pulsación
					if ( auxData == BOTON_1 )
						eint1_gestionar_pulsacion(); // EINT1
					else 
						eint2_gestionar_pulsacion(); // EINT2	
					break;
					
				case POWER_DOWN:
					// pasar a estado power-down
					power_hal_deep_sleep();
					break;
				
				case ev_VISUALIZAR_CUENTA:
					visualizar_cuenta(auxData);
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
		// set EINT1 y EINT2 pin on pin block to input
	gpio_hal_sentido(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS, GPIO_HAL_PIN_DIR_OUTPUT);
}


__inline void activar_overflow_gpio_pin(void) {
		// escribir en GPIO31
	gpio_hal_escribir(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS, 1);
	
}

#if TEST_CONSUMO | TEST_BOTONES
	void set_retardo_USUARIO_AUSENTE(const unsigned int time)
	{
		USUARIO_AUSENTE = time;
	}
#endif

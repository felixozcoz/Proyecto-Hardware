#include "planificador.h"
#include "visualizar.h"


// Tiempo para determinar "sin actividad de usuario"
// para pasar a estado power-down del procesador
static unsigned int USUARIO_AUSENTE = 12000; // en ms


// (Función auxiliar)
//
// Activa el pin de overflow en la GPIO
void activar_overflow_gpio_pin(void);

// (Función auxiliar)
//
// Inicializa GPIO con GPIO31
// (overflow bit) a output dir
void GPIO_inicializar(void);


// Función privada (modularización)
//
// Inicializa los módulos necesarios
void inicializar_modulos(void);

// Función privada (modularización)
//
// Gestiona los eventos
void gestionar_eventos(const uint32_t periodo_timer1);




// **** FUNCIONES ****

// Inicilizar cola de eventos
void inicializar_cola_eventos(const uint32_t periodo_timer1) 
{
	inicializar_modulos();
	
	alarma_activar(POWER_DOWN, USUARIO_AUSENTE, 0); // alarma power-down proccessor mode
	
	gestionar_eventos(periodo_timer1);
}



void inicializar_modulos(void)
{
	GPIO_inicializar();
	
	#if  ! ( TEST_ALARMAS | TEST_BOTONES )
	
		FIFO_inicializar(GPIO_OVERFLOW);
		alarma_inicializar(); 
	
	#endif 
	
	#if JUEGO
	
		inicializar_juego(0, 0);
		inicializar_visualizar(GPIO_JUEGO, GPIO_JUEGO_BITS);
	
	#endif
	
	#if HELLO_WORLD_DEMO
	
		hello_world_inicializar();
		inicializar_visualizar(GPIO_HELLO_WORLD, GPIO_HELLO_WORLD_BITS);
	
	#endif
	
	inicializar_botones(); 
	
}


void gestionar_eventos(const uint32_t periodo_timer1)
{
	EVENTO_T evento = EVENTO_VOID; 
	uint32_t auxData = 0;
	
	while(1){
			evento = EVENTO_VOID; // reset evento
		
			FIFO_extraer(&evento, &auxData);
		
			switch(evento){
				
				case ev_LATIDO:
					hello_world_tratar_evento();
					break;
				
				case ev_VISUALIZAR_HELLO:
					visualizar_cuenta(auxData);
					break;
				
				case HELLO_OVERFLOW:
					activar_overflow_gpio_pin();
					while(1);
				
				case PULSACION:
					alarma_activar(POWER_DOWN, USUARIO_AUSENTE, 0); // reset alarma usuario ausente
				
						// revisar estado de botones
					if ( auxData == BOTON_1 ) eint1_gestionar_pulsacion(); // EINT1
					else 	eint2_gestionar_pulsacion(); // EINT2	
				
					break;
				
				case DESPULSACION:
					#if JUEGO 
						juego_tratar_evento(ev_VISUALIZAR_CUENTA, auxData);
					#endif 
					break;
					
				case POWER_DOWN:
					power_hal_deep_sleep();
					break;
				
				case ev_VISUALIZAR_CUENTA:
					visualizar_cuenta(auxData);
					break;
				
				case REVISAR_ALARMAS:
					alarma_tratar_evento(periodo_timer1); // check alarmas
					break;
				
				case ALARMAS_OVERFLOW:
					activar_overflow_gpio_pin();
					while(1);
				
				default: 
					// para tratar eventos de alarmas
					#if TEST_ALARMAS
						if( evento < 0) break;
					#endif
				
					power_hal_wait(); // cambio modo IDLE del procesador
					break;
			}
	}
}


// **** Funciones Auxiliares ****

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

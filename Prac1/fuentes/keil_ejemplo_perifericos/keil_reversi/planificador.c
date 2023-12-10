#include "planificador.h"
#include "visualizar.h"
#include "alarmas.h"
#include "botones.h"
#include "hello_world.h"
#include "juego.h"
#include "cola_FIFO.h"
#include "Power_management.h"
#include "GPIO_hal.h"
#include "io_reserva.h"
#include "temporizador_drv.h"
#include "linea_serie_drv.h"
#include "tablero_test.h"

// Tiempo para determinar "sin actividad de usuario"
// para pasar a estado power-down del procesador
static unsigned int USUARIO_AUSENTE = 12000; // en ms


// (Funci�n auxiliar)
//
// Activa el pin de overflow en la GPIO
void activar_overflow_gpio_pin(void);

// (Funci�n auxiliar)
//
// Inicializa GPIO con GPIO31
// (overflow bit) a output dir
void GPIO_inicializar(void);


// Funci�n privada (modularizaci�n)
//
// Inicializa los m�dulos necesarios
void inicializar_modulos(void);

// Funci�n privada (modularizaci�n)
//
// Gestiona los eventos
void gestionar_eventos(const uint32_t periodo_timer1);

// Funci�n privada
//
// Inicializa los test espec�ficos
// para los test unitarios de m�dulos
// Flag "TESTING" debe estar a 1
// Resto de flags de control de test en "../Tests/test.h"
void init_modulos_test(void);


// **** FUNCIONES ****

// Iniciar planificador
void planificador(const uint32_t periodo_timer1) 
{
	inicializar_modulos();
	
	alarma_activar(ev_POWER_DOWN, USUARIO_AUSENTE, 0); // alarma power-down proccessor mode
	temporizador_drv_reloj(periodo_timer1, FIFO_encolar, ev_REVISAR_ALARMAS); // inicializar reloj 
	
	gestionar_eventos(periodo_timer1);
}



void inicializar_modulos(void)
{
	GPIO_inicializar();
	inicializar_botones(); 
	inicializar_juego(tablero_test7, GPIO_JUEGO);
	
	#if TESTING	
		init_modulos_test();
	#endif
	
	// inicializar m�dulos restante seg�n conveniencia
	// inicializar_juego(0, 0);
	// hello_world_inicializar(..., ...);
	// inicializar_visualizar(..., ...);
}


void gestionar_eventos(const uint32_t periodo_timer1)
{
	EVENTO_T evento = ev_EVENTO_VOID; 
	uint32_t auxData = 0;
	
	while(1){
			evento = ev_EVENTO_VOID; // reset evento
		
			FIFO_extraer(&evento, &auxData);
		
			switch(evento){
				
				case ev_LATIDO:
					hello_world_tratar_evento();
					break;
				
				case ev_VISUALIZAR_HELLO:
					visualizar_cuenta(auxData);
					break;
				
				case ev_PULSACION:
					alarma_activar(ev_POWER_DOWN, USUARIO_AUSENTE, 0); // reset alarma usuario ausente
				
					// revisar estado de botones
					if ( auxData == BOTON_1 ) eint1_gestionar_pulsacion(); // EINT1
					else 	eint2_gestionar_pulsacion(); // EINT2	
				
					break;
				
				case ev_DESPULSACION:
					juego_tratar_evento(ev_DESPULSACION, auxData);
					break;

				case ev_VISUALIZAR_CUENTA:
					visualizar_cuenta(auxData);
					break;
				
				case ev_REVISAR_ALARMAS:
					alarma_tratar_evento(periodo_timer1); // comprobar alarmas
					break;
				
				case ev_ALARMAS_OVERFLOW:
					activar_overflow_gpio_pin();	// no hay m�s alarmas disponibles
					while(1);
				
				case ev_RX_SERIE:
					if (auxData == trama_TAB )
						juego_tratar_evento(ev_RX_SERIE, auxData);
					break;
				
				case ev_TX_SERIE:
					// ...
					break;
				
				case ev_POWER_DOWN:
					PM_power_down(); // cambiar modo de estado del procesador a power-down (deep-sleep)
					break;
				
				default: 
					// para tratar eventos de alarmas
					#if TESTING && TEST_ALARMAS 
						if( evento < 0) break;
					#endif
				
					PM_idle(); // cambio modo de estado del procesador a resposo (IDLE)
					break;
			}
	}
}


// **** Funciones Auxiliares ****

void GPIO_inicializar(void)
{
	gpio_hal_iniciar();
		// set GPIO31 (overflow) to output dir
	gpio_hal_sentido(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS, GPIO_HAL_PIN_DIR_OUTPUT);
		// set EINT1 y EINT2 pin on pin block to input ???
	gpio_hal_sentido(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS, GPIO_HAL_PIN_DIR_OUTPUT);
}


__inline void activar_overflow_gpio_pin(void) {
		// escribir en GPIO31
	gpio_hal_escribir(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS, 1);
	
}


void init_modulos_test(void){
		#if  ! ( TEST_ALARMAS | TEST_BOTONES )
		FIFO_inicializar(GPIO_OVERFLOW);
		alarma_inicializar(); 
	#endif 
	#if JUEGO
		inicializar_juego(tablero_test7);
		inicializar_visualizar(GPIO_JUEGO, GPIO_JUEGO_BITS);
	#endif
	#if HELLO_WORLD_DEMO
		hello_world_inicializar(GPIO_HELLO_WORLD, GPIO_HELLO_WORLD_BITS);
		inicializar_visualizar(GPIO_HELLO_WORLD, GPIO_HELLO_WORLD_BITS);
	#endif	
}



#if TESTING && (TEST_CONSUMO | TEST_BOTONES)
	void set_retardo_USUARIO_AUSENTE(const unsigned int time)
	{
		USUARIO_AUSENTE = time;
	}
#endif

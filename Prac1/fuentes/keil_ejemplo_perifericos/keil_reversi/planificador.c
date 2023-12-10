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
#include "cola_mensajes.h"

// Tiempo para determinar "sin actividad de usuario"
// para pasar a estado power-down del procesador
static unsigned int USUARIO_AUSENTE = 12000; // en ms


// (Función auxiliar)
//
// Activa el pin de overflow en la GPIO
void activar_overflow_gpio_pin(void);

// Función privada (modularización)
//
// Inicializa los módulos necesarios
void inicializar_modulos(void);

// Función privada (modularización)
//
// Gestiona los eventos
void gestionar_eventos(const uint32_t periodo_timer1);

// Función privada
//
// Inicializa los test específicos
// para los test unitarios de módulos
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


// FALTA WATCHDOG
void inicializar_modulos(void)
{
	gpio_hal_iniciar();
	alarma_inicializar();
	FIFO_inicializar(GPIO_OVERFLOW);
	inicializar_botones();
	inicializar_cola_mensajes(GPIO_OVERFLOW);
	iniciar_serial(GPIO_SERIE_ERROR, GPIO_SERIE_ERROR_BITS);
	inicializar_juego(tablero_test7, GPIO_JUEGO); // USANDO TABLERO DE TEST PREDETERMINADO
	
	#if TESTING	
		init_modulos_test();
	#endif
	
	// inicializar módulos restante según conveniencia
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
					activar_overflow_gpio_pin();	// no hay más alarmas disponibles
					while(1);
				
				case ev_RX_SERIE:
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

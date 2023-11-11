                  
#include <LPC210x.H>                       /* LPC210x definitions */
#include "Power_management.h"
#include "temporizador_drv.h"
#include "planificador.h"
#include "tests.h"
#include "botones.h"
#include "alarmas.h"


#define TESTING 1



int main(){
	
	#if ! TESTING
	
		// ... 
	
	#else

		uint32_t periodo_timer1_ms __attribute__((unused)) = 10;
	
		#if TEST_FIFO
				
			test_FIFO();
	
		#elif TEST_GPIO
	
	
		#elif TEST_ALARMAS
		
			test_alarmas();
					// activar reloj
			temporizador_drv_reloj(periodo_timer1_ms, FIFO_encolar, REVISAR_ALARMAS);
			
			inicializar_cola_eventos(periodo_timer1_ms);

		#elif TEST_BOTONES
			
			test_botones();
			
			temporizador_drv_reloj(periodo_timer1_ms, FIFO_encolar, REVISAR_ALARMAS);
			
			inicializar_cola_eventos(periodo_timer1_ms);
		
		#elif TEST_CONSUMO
				// settear tiempo en ms del delay de usuario ausente
			set_retardo_USUARIO_AUSENTE(20); // en ms
		
			temporizador_drv_reloj(periodo_timer1_ms, FIFO_encolar, REVISAR_ALARMAS);
			
			inicializar_cola_eventos(periodo_timer1_ms);
		
		#elif DEMOSTRADOR
		
			temporizador_drv_reloj(periodo_timer1_ms, FIFO_encolar, REVISAR_ALARMAS);
			
			inicializar_cola_eventos(periodo_timer1_ms);
			
		
		#endif
		
	#endif
	
	while(1);
}


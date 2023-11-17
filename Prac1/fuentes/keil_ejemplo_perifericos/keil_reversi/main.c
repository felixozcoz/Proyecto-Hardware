                  
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
	
		#elif TEST_ALARMAS
			test_alarmas();
	
		#elif TEST_BOTONES | TEST_CONSUMO
				// settear tiempo en ms del delay de usuario ausente
			set_retardo_USUARIO_AUSENTE(50); // en ms
		
		#endif
	#endif
	
	temporizador_drv_reloj(periodo_timer1_ms, FIFO_encolar, REVISAR_ALARMAS);
	inicializar_cola_eventos(periodo_timer1_ms);
	
	while(1);
}


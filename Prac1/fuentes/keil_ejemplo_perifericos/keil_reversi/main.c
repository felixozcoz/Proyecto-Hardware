                  
#include "planificador.h"
#include "linea_serie_drv.h"

int main(){

	#if ! TESTING
		char miMsg[] = "ABCDE";
		iniciar_serial(GPIO_SERIE_ERROR, GPIO_SERIE_ERROR_BITS);
		linea_serie_drv_enviar_array(miMsg);
	#else
		uint32_t periodo_timer1_ms __attribute__((unused)) = 10;
	
		#if TEST_FIFO
			test_FIFO(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS);	
	
		#elif TEST_ALARMAS
			test_alarmas();
	
		#elif TEST_BOTONES | TEST_CONSUMO
				// settear tiempo en ms del delay de usuario ausente
			set_retardo_USUARIO_AUSENTE(50); // en ms
	
		#elif TEST_WATCHDOG
			WD_hal_test(1);
		#endif

		planificador(periodo_timer1_ms);
	#endif

	while(1);
}


                  
#include <LPC210x.H>                       /* LPC210x definitions */
#include "Power_management.h"
#include "temporizador_drv.h"
#include "planificador.h"
#include "tests.h"
#include "botones.h"
#include "alarmas.h"
#include "WD_hal.h"
#include "SWI_llamadas.h"
#include "linea_serie_drv.h"

#define TESTING 1

// leer bit I del SPSR (bit I es IRQ interrupt disable bit)
int __swi(0xFC) read_IRQ_bit(void);

int main(){

	#if ! TESTING
		char miMsg[] = "ABCDE";
		iniciar_serial();
		linea_serie_drv_enviar_array(miMsg);
	#else
		uint32_t periodo_timer1_ms __attribute__((unused)) = 10;
	
		#if TEST_FIFO
			test_FIFO();	
	
		#elif TEST_ALARMAS
			test_alarmas();
	
		#elif TEST_BOTONES | TEST_CONSUMO
				// settear tiempo en ms del delay de usuario ausente
			set_retardo_USUARIO_AUSENTE(50); // en ms
	
		#elif TEST_WATCHDOG
			WD_hal_test(1);
		#endif
	
		temporizador_drv_reloj(periodo_timer1_ms, FIFO_encolar, REVISAR_ALARMAS);
		inicializar_cola_eventos(periodo_timer1_ms);
		
		
		
		while(1);
	#endif
	

	
	
}


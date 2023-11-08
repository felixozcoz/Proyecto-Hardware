                  
#include <LPC210x.H>                       /* LPC210x definitions */
#include "timer0.h"
#include "Power_management.h"
#include "boton_eint0.h"
#include "temporizador_drv.h"
#include "planificador.h"
#include "tests.h"
#include "GPIO.h"
#include "alarmas.h"
#include <limits.h>


#if TEST_ALARMAS

// MAIN: Test
int main(){
	uint32_t periodo_timer1_ms = 10;
	test_alarmas();
			// activar reloj
	temporizador_drv_reloj(periodo_timer1_ms, FIFO_encolar, REVISAR_ALARMAS);
	
	inicializar_cola_eventos(periodo_timer1_ms);

	while(1);
}

#else 

// MAIN: Hello world
//int main() {
//	// inicialiar timer, interrumpe cada 10ms
//	temporizador_drv_reloj(10, FIFO_encolar, EVENTO_HELLO_WORLD);
//	
//	// inicializar planificador
//	inicializar_cola_eventos();
//	
//	while(1);
//}

#endif


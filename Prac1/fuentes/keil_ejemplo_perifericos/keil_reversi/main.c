#include "planificador.h"
#include "linea_serie_drv.h"
#include "juego.h"
#include "tablero_test.h"
#include <string.h>
#include "cola_mensajes.h"

#define TESTING 0 // activar para realizar test de módulos; flags de test en "test.h"
void tests(void); // función de test de módulos


// Función principal
int main(){
	#if TESTING
		tests();
	#endif

	planificador(10);
	while(1);
}



// ***** función de test de módulos *****

void tests(void){
	#include "GPIO_hal.h"
	#include "io_reserva.h"
	#include "tests.h" // contiene los flags de control de flujo
	
	uint32_t periodo_timer1_ms __attribute__((unused)) = 10;
	gpio_hal_iniciar();
	
	#if TEST_FIFO
		test_FIFO(GPIO_OVERFLOW);	
	#elif TEST_ALARMAS
		test_alarmas();
	#elif TEST_BOTONES | TEST_CONSUMO
			// settear tiempo en ms del delay de usuario ausente
		set_retardo_USUARIO_AUSENTE(50); // en ms
		planificador(periodo_timer1_ms);
	#elif TEST_WATCHDOG
		WD_hal_test(1);
	#elif TEST_COLA_MENSAJES
		iniciar_serial(GPIO_SERIE_ERROR, GPIO_SERIE_ERROR_BITS);
		test_cola_mensajes(GPIO_OVERFLOW);
	#endif
	
	#if TEST_ALARMAS | TEST_BOTONES | TEST_CONSUMO
		planificador(periodo_timer1_ms);
	#endif
	while(1);
}


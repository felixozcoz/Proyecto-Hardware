// test_alarmas.c
#include "alarmas.h"
#include "cola_FIFO.h"
#include "temporizador_drv.h"
#include "planificador.h"
#include "tests.h"

#define TEST_NORMAL 0
#define TEST_PERIODICA_CANCELAR 0
#define TEST_ALARMA_OVERFLOW 1


// Parámetros de test
const uint32_t retardo_10ms  = 0x0000000A;
// los retardos son de 100ms
const uint32_t retardo_cancelar = 0;
const uint32_t retardo_periodica = 0xA8000000;
const uint32_t retardo_normal = 0x00000064;	

const uint32_t auxData_test = 0;

	
void test_alarmas(void){
	#if TEST_NORMAL
			// programar alarma no periódica con 10 ms de retardo
			// objetivo: disparar evento
		alarma_activar(EVENTO_FICTICIO_1, retardo_10ms, auxData_test);
	#endif

	#if TEST_PERIODICA_CANCELAR
			// programar alarma  periodica con 2^28ms de retardo
			// objetivo: aumentar contador de alarma (no disparar)
		alarma_activar(EVENTO_FICTICIO_2, retardo_periodica, auxData_test);
		
			// objetivo: cancelar una alarma
		alarma_activar(EVENTO_FICTICIO_2, retardo_cancelar, auxData_test);
	#endif
	
	#if TEST_ALARMA_OVERFLOW
			// overflow por creación de alarmas
			// objetivo: visualizar en la GPIO el resultado de overflow
		alarma_activar(EVENTO_FICTICIO_1, retardo_periodica, auxData_test);
		alarma_activar(EVENTO_FICTICIO_2, retardo_periodica, auxData_test);
		alarma_activar(EVENTO_FICTICIO_3, retardo_periodica, auxData_test);
		alarma_activar(EVENTO_FICTICIO_4, retardo_normal, auxData_test); 
		alarma_activar(EVENTO_FICTICIO_5, retardo_normal, auxData_test); 
	#endif
		
}

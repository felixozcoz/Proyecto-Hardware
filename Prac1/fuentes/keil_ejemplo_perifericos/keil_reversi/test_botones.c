// test_botones.h

// Test para las interrupciones 
// externas EINT1 y EINT2 que simulan
// botones

#include "tests.h"
#include "alarmas.h"
#include "botones.h"



// Comprobar a mano, porque no sé como hacerlo automático
// Comprobar casos:
//	- Desactivar interrupciones
//	- Reactivar interrupciones
void test_botones(void)
{
	
	uint32_t retardo_per_10ms = 0x8000000A;
	
	// activar alarmas de los botones
	alarma_activar(PULSACION, retardo_per_10ms, BOTON_1);
	alarma_activar(PULSACION, retardo_per_10ms, BOTON_2);
}





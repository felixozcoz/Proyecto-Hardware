#include <inttypes.h>
#include "hello_world.h"
#include "io_reserva.h"
#include "alarmas.h"

// Contador para indicar el led
static uint8_t contador = 0;

#define heartbeat_ms 0x8000000A

void hello_world_inicializar(void) 
{
	contador = 0;
	alarma_activar(ev_LATIDO, heartbeat_ms, 0); // heartbeat (periódico) cada 10ms
}


void hello_world_tratar_evento(void)
{
	if(contador == 0xFF) {
		FIFO_encolar(HELLO_OVERFLOW, 0);
		return; 
	}
	
	contador++;
	contador = contador % 256;
	
	FIFO_encolar(ev_VISUALIZAR_HELLO, contador);
}

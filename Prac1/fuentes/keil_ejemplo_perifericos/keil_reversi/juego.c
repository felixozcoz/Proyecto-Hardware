// juego.c

#include <LPC210x.h>
#include "juego.h"
#include "cola_FIFO.h"
#include "temporizador_drv.h"

#include <inttypes.h>

	// contabiliza las pulsaciones de los botone
	// pulsar EINT1 incrementa en una unidad y
	// pulsar EINT2 decrementa en una unidad
static int cuenta;
	// guarda el tiempo transcurrido
	// entre las dos últimas pulsaciones
static unsigned int	intervalo;

// Ultima pulsación
static unsigned int last_press = 0;

void inicializar_juego(const int _cuenta, const unsigned int _intervalo){
	cuenta = _cuenta;
	intervalo = _intervalo;
}


void juego_tratar_evento(const EVENTO_T ID_evento, const uint32_t auxData){
	unsigned int now = temporizador1_leer_drv();
	intervalo = (now - last_press);
	last_press = now;
	
	if(auxData == 1){
		cuenta ++;
	} else if(auxData == 2){
		cuenta--;
	}
	
	FIFO_encolar(ev_VISUALIZAR_CUENTA, cuenta);
}

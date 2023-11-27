#include <LPC210x.h>

#include "juego.h"
#include "cola_FIFO.h"
#include "temporizador_drv.h"
#include "botones.h"
#include "tablero.h"
#include "conecta_K_2023.h"

// Contabiliza las pulsaciones de los botone
// pulsar EINT1 incrementa en una unidad y
// pulsar EINT2 decrementa en una unidad
static int cuenta;

// Guarda el tiempo transcurrido
// entre las dos últimas pulsaciones
static unsigned int	__attribute__((unused)) intervalo; // retirar el "__attribute__((unused))" cuando se utilice intervalo

// Ultima pulsación
static unsigned int last_press = 0;


// Almacenar tablero??


void inicializar_juego(TABLERO tablero){
	// inicializar tablero (de momento con uno de test)
	tablero_inicializar(&tablero);
	
}


void juego_tratar_evento(const EVENTO_T ID_evento, const uint32_t auxData){
//	unsigned int now = clock_get_us(); // en ms
//	intervalo = (now - last_press);
//	last_press = now;
//	
//	if(auxData == BOTON_1)	// EINT1
//		cuenta ++;
//	else	// EINT2
//		cuenta--;
//	
//	FIFO_encolar(ev_VISUALIZAR_CUENTA, cuenta); // visualizar cuenta en GPIO
	
	// recibido ev_RX_SERIE
	if (ID_evento == ev_RX_SERIE)
			conecta_K_visualizar_tablero();

}
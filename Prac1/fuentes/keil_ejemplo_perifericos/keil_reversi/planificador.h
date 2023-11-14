#ifndef PLANIFICADOR_H
#define PLANIFICADOR_H

#include <inttypes.h>

#include "planificador.h"
	
#include "alarmas.h"
#include "botones.h"
#include "cola_FIFO.h"
#include "GPIO.h"
#include "power.h"

#include "hello_world.h"
#include "juego.h"
#include "visualizar.h"

#include "tests.h"

#include "io_reserva.h"


// Inicializar cola de eventos
void inicializar_cola_eventos(const uint32_t periodo_timer1);

#if TEST_CONSUMO | TEST_BOTONES
	void set_retardo_USUARIO_AUSENTE(const unsigned int time);
#endif








#endif // PLANIFICADOR_H


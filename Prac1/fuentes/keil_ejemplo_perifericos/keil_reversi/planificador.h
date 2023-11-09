#ifndef PLANIFICADOR_H
#define PLANIFICADOR_H

#include <stdint.h>
#include "GPIO.h"
#include "cola_fifo.h"
#include "io_reserva.h"

// Inicializar cola de eventos
void inicializar_cola_eventos(const uint32_t periodo_timer1);

#if TEST_CONSUMO
	void set_retardo_USUARIO_AUSENTE(const unsigned int time);
#endif








#endif // PLANIFICADOR_H


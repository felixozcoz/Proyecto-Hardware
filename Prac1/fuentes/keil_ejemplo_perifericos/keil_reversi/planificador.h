#ifndef PLANIFICADOR_H
#define PLANIFICADOR_H

#include <inttypes.h>

#include "planificador.h"	
#include "tests.h"	// fichero que contiene flags de compilación para realizar test unitarios de módulos

// Iniciar planificador
void planificador(const uint32_t periodo_timer1);

#if TEST_CONSUMO | TEST_BOTONES
	void set_retardo_USUARIO_AUSENTE(const unsigned int time);
#endif








#endif // PLANIFICADOR_H


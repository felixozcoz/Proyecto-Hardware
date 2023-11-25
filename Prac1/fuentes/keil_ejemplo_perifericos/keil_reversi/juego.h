// juego.h

#ifndef __JUEGO_H__
#define __JUEGO_H__

#include <inttypes.h>
#include "EVENTOS_T.h"

void inicializar_juego(const int _cuenta, const unsigned int _intervalo);

void juego_tratar_evento(const EVENTO_T ID_evento, const uint32_t auxData);

#endif // __JUEGO_H__

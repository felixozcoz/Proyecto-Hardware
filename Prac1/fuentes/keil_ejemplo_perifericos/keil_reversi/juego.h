// juego.h

#ifndef __JUEGO_H__
#define __JUEGO_H__

#include <inttypes.h>
#include "EVENTOS_T.h"
#include "tablero.h"

void inicializar_juego(TABLERO tablero);

void juego_tratar_evento(const EVENTO_T ID_evento, const uint32_t auxData);

#endif // __JUEGO_H__

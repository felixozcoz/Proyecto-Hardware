// juego.h

#ifndef __JUEGO_H__
#define __JUEGO_H__

#include <inttypes.h>
#include "EVENTOS_T.h"
#include "tablero.h"
#include "config_conecta_K.h"


void inicializar_juego(uint8_t tab_input[NUM_FILAS][NUM_COLUMNAS]);

void juego_tratar_evento(const EVENTO_T ID_evento, const uint32_t auxData);

#endif // __JUEGO_H__

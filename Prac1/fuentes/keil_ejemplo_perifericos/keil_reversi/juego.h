// juego.h

#ifndef __JUEGO_H__
#define __JUEGO_H__

#include <inttypes.h>
#include "EVENTOS_T.h"
#include "tablero.h"
#include "config_conecta_K.h"


void inicializar_juego(uint8_t tab_input[NUM_FILAS][NUM_COLUMNAS], const GPIO_HAL_PIN_T _pin_cmd_no_valido);

void juego_tratar_evento(const EVENTO_T ID_evento, const uint32_t auxData);

#endif // __JUEGO_H__

#ifndef ENTRADA_H
#define ENTRADA_H

#include <inttypes.h>
#include "entrada.h"
#include "config_conecta_K.h"
	
uint8_t n_jugadas_test = 8;	
	
// Entradas de test 

//	uint8_t entrada[ALIGN_8]
// 		entrada[0] = 0; //nueva
//		entrada[1] = 0; //fila
//		entrada[2] = 0; //columna
//		entrada[3] = 0; //color

// Entradas para comprobar esquinas
typedef uint8_t entradasType[ALIGN_8];

static entradasType entradas_test[] = 
{ 
	// Entradas sin victoria
{ 1, 1, 1, 1 , 0, 0, 0, 0 },	// esquina superior izquierda
{ 1, 1, NUM_COLUMNAS, 1 , 0, 0, 0, 0},	// esquina superior derecha
{ 1, NUM_FILAS, 1, 1 , 0, 0, 0, 0},	// esquina inferior izquierda

// Entradas que provoca victoria
{ 1, NUM_FILAS, NUM_COLUMNAS, 1 , 0, 0, 0, 0},	// esquina inferior derecha
{ 1, 3, 6, 2 , 0, 0, 0, 0}, 	// vertical
{ 1, 2, 4, 2 , 0, 0, 0, 0},		// horizontal
{ 1, 4, 4, 2 , 0, 0, 0, 0}, 	// diagonal positiva sobre eje x
{ 1, 5, 4, 1 , 0, 0, 0, 0}  	// diagonal negativa sobre eje x
};

#endif

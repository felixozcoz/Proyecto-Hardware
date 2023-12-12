#ifndef _TRAMAS_H_
#define _TRAMAS_H_

#define MAX_LEN_TRAMA 3

#include <inttypes.h>
#include <stdbool.h>

#define CARACTER_INICIO_TRAMA '$'
#define CARACTER_FIN_TRAMA '!'

// Tramas válidas en el juego
typedef enum { 
	trama_NEW = ('N' << 16) | ('E' << 8) | 'W', 
	trama_END = ('E' << 16) | ('N' << 8) | 'D',
	trama_JUGADA = (0 << 16) | ('-' << 8) | 0,
} Trama_t;


// tramaToInt
//
// Parámetros:
// trama_buffer: contiene una trama
//
// Devuelve 'trama_buffer' codificada en ASCII
uint32_t tramaToInt(char trama_buffer[MAX_LEN_TRAMA]);


// tramaToString
//
// Parámetros:
// _trama: contiene una trama codificado en ASCII 
// res: alamacena el resultado de la conversión de '_trama'
//	    en un string
//
// Devuelve 'trama_buffer' en caracteres ASCII en 'res'
void tramaToString(const uint32_t _trama, char res[MAX_LEN_TRAMA]);

#endif // _TRAMAS_H_

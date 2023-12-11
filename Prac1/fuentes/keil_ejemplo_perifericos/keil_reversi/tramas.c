#include "tramas.h"

#include <stdlib.h>

uint32_t tramaToInt(char trama_buffer[MAX_LEN_TRAMA]){
	uint32_t trama = 0;
	size_t i;
	
	for (i = 0; i < MAX_LEN_TRAMA; i++)
		trama |= trama_buffer[i] << (8* (MAX_LEN_TRAMA - 1 - i));
	
	return trama;
}

void tramaToString(const uint32_t _trama, char res[MAX_LEN_TRAMA]){
		size_t i;
		char ch;
		int kuka;
		for (i = 0; i < MAX_LEN_TRAMA; i++){
			kuka = ( (_trama >> (8 * (MAX_LEN_TRAMA - 1 - i)) ) & 0xFF );
			ch = (char)(kuka);
			res[i] = ch;
		}
		res[MAX_LEN_TRAMA] = '\0';
}

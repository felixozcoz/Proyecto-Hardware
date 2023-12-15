#ifndef _MENSAJE_T_H_
#define _MENSAJE_T_H_

#include <string.h>

#define MAX_MENSAJE_LENGTH 300

// Definici�n del struct mensaje
typedef char Mensaje_t[MAX_MENSAJE_LENGTH] __attribute__((aligned(4)));

// Funci�n para settear el valor del mensaje en un momento posterior
__inline void setMensaje(Mensaje_t mensaje, const char *valor) {
		strncpy(mensaje, valor, MAX_MENSAJE_LENGTH - 1);
		mensaje[MAX_MENSAJE_LENGTH-1] = '\0';  // Asegura la terminaci�n nula por si valor es m�s corta
}

#endif // _MENSAJE_T_H_

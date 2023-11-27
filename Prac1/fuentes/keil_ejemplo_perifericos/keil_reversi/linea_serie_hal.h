#ifndef _LINEA_SERIE_HAL_H_
#define _LINEA_SERIE_HAL_H_

// Inicializa la línea serie
// Parámetros:
//	respectivamente, las funciones
// 	de callback para las interrupciones
//	de la UART0
void iniciar_serial_hal( void(*_funcion_callback_transmision)(), void(*_funcion_callback_recepcion)() );

// Transmite 'ch' por línea serie
void sendchar_serie(char ch);

// Devuelve un caracter leído de 
// línea serie
int getchar_serie(void);

#endif // _LINEA_SERIE_HAL_H_

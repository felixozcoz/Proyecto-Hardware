#ifndef _IO_RESERVA_H_
#define _IO_RESERVA_H_

#define GPIO_button 12

// Leds
#define GPIO_LED_0 16
#define GPIO_LED_1 17
#define GPIO_LED_2 18
#define GPIO_LED_3 19
#define GPIO_LED_4 20
#define GPIO_LED_5 21
#define GPIO_LED_6 22
#define GPIO_LED_7 23

#define GPIO_LEDS_BITS 8

// Bits overflow
#define GPIO_OVERFLOW 31
#define GPIO_OVERFLOW_BITS 1

// Prueba "Hello World"
#define GPIO_HELLO_WORLD 0
#define GPIO_HELLO_WORLD_BITS 8

// Para el juego
#define GPIO_JUEGO 16
#define GPIO_JUEGO_BITS 8

// Bits EINT's
#define GPIO_EINT1 14
#define GPIO_BITS_EINT1 1
#define GPIO_EINT2 15
#define GPIO_BITS_EINT2 1

// Linea serie (UART0)
#define GPIO_SERIE_ERROR 30
#define GPIO_SERIE_ERROR_BITS 1

#endif // _IO_RESERVA_H_

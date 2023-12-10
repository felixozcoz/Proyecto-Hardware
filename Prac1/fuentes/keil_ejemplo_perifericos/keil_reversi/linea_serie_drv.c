#include "linea_serie_drv.h"

#include "linea_serie_hal.h"
#include "EVENTOS_T.h"
#include "cola_FIFO.h"
#include <string.h>
#include "cola_mensajes.h"

#define MAX_LEN_TRAMA 3

enum{ inicio, procesando};
static volatile unsigned int ESTADO = inicio;

// Buffer de trama
static volatile char trama_buffer[MAX_LEN_TRAMA];
static volatile	size_t trama_len_buff;

// índice dentro de la cola que indica el mensaje
static Mensaje_t msg_enviar;
// indica el caracter por el que va escribiendo el mensaje actual
static int index; 


// Pins de GPIO utilizados por línea serie
static GPIO_HAL_PIN_T pin_inicial;
static GPIO_HAL_PIN_T num_pins;

// (Func privada)
void linea_serie_drv_continuar_envio(void);


void iniciar_serial(const GPIO_HAL_PIN_T _pin, const GPIO_HAL_PIN_T _num_pins){
		// set GPIO pins
	pin_inicial = _pin;
	num_pins = _num_pins;
	gpio_hal_sentido(pin_inicial, num_pins, GPIO_HAL_PIN_DIR_OUTPUT);
		// set vars gestión mensajes
	index = -1;
		// init hal
	iniciar_serial_hal(linea_serie_drv_continuar_envio ,gestor_serial);
}

// se llama cada vez que se quiere procesar un caracter
void gestor_serial(void)
{
	int ch = getchar_serie();	// leer dato línea serie
	
	switch(ESTADO){
		
		case inicio:
			if ( ch == '$' ){
				gpio_hal_escribir(pin_inicial, num_pins, 0); // desactivar GPIO30
				trama_len_buff = 0;
				ESTADO = procesando;
			}
			break;
		
		case procesando:
			
			if( ch == '!' )	// caracter fin de comando
			{
				int32_t trama = (trama_buffer[0] << 16) | (trama_buffer[1] << 8) | trama_buffer[2]; // almacenar comando
				
					// nueva partida
				if( trama_buffer[0] == 'N' && trama_buffer[1] == 'E' && trama_buffer[2] == 'W'){
						FIFO_encolar(ev_RX_SERIE, trama);	// atómico
				}
					// terminar partida
				else if( trama_buffer[0] == 'E' && trama_buffer[1] == 'N' && trama_buffer[2] == 'D'){
						FIFO_encolar(ev_RX_SERIE, trama); // atómico
				}
					// indica visualizar tablero
				else if( trama_buffer[0] == 'T' && trama_buffer[1] == 'A' && trama_buffer[2] == 'B'){
						FIFO_encolar(ev_RX_SERIE, trama); // atómico
				}
					// indica jugada introducida
				else if( trama_buffer[1] == '-'){
					if( trama_buffer[0] >= '1' && trama_buffer[0] <= '7' &&  trama_buffer[2] >= '1' && trama_buffer[2] <= '7' )
							FIFO_encolar(ev_RX_SERIE, trama); // atómico
				}
				ESTADO = inicio; // trama válida o no vuelve a inicio
			}
			else{
				if( trama_len_buff < MAX_LEN_TRAMA )
					trama_buffer[trama_len_buff++] = ch;	// añadir cadena al buffer
				else{			
					gpio_hal_escribir(pin_inicial, num_pins, 1); // enceder led GPIO30 "GPIO_SERIE_ERROR"
					ESTADO = inicio; // secuencia de cadenas incongruente con tramas fuera del dominio
				}
			}
			break;
	}
}

// Inicializa estructura de gestión de mensajes
// por línea serie y envíar primer caracter
void linea_serie_drv_enviar_array(Mensaje_t msg)
{
		// mensaje vacío
	if(! strlen(msg) )
		return; 
	
		// si se está tratando un mensaje: encolar
	if (index > -1)
		encolar_msg(msg);
		// si cola de mensajes vacía: tratar
	else if( estaVacia_msg() || index == -1){
			// inicializar variables de mensaje
		strncpy(msg_enviar, msg, strlen(msg)+1);
		index = 0;
			// enviar primer caracter
		sendchar_serie(msg[index]);
		index++;
	}
	else
		encolar_msg(msg);	
}

// Transmite el resto de caracteres del mensaje
void linea_serie_drv_continuar_envio(void)
{
	// Verificar si hemos alcanzado el final del buffer
    if (index < strlen(msg_enviar)) {
				sendchar_serie(msg_enviar[index]);
        index++; 	
    } else {
      FIFO_encolar(ev_TX_SERIE, 0); // transmisión del mensaje completo finalizada
			index = -1;	
			if( ! estaVacia_msg() ){
					desencolar_msg(&msg_enviar);
					linea_serie_drv_enviar_array(msg_enviar);
				}
		}
}

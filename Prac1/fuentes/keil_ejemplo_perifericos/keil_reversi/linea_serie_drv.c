#include "linea_serie_drv.h"

#include "linea_serie_hal.h"
#include "EVENTOS_T.h"
#include "cola_FIFO.h"
#include <string.h>

#define MAX_LEN_TRAMA 3

enum{ inicio, procesando};
static volatile unsigned int ESTADO = inicio;

// Buffer de trama
static volatile char trama_buffer[MAX_LEN_TRAMA];
static volatile	size_t trama_len_buff;

// Gesti�n de mensajes por l�nea serie
static volatile char *buffer;
static volatile size_t len_buff;
static volatile size_t index;


// Pins de GPIO utilizados por l�nea serie
static GPIO_HAL_PIN_T pin_inicial;
static GPIO_HAL_PIN_T num_pins;

// (Func privada)
void linea_serie_drv_continuar_envio(void);


void iniciar_serial(const GPIO_HAL_PIN_T _pin, const GPIO_HAL_PIN_T _num_pins){
		// set GPIO pins
	pin_inicial = _pin;
	num_pins = _num_pins;
	gpio_hal_sentido(pin_inicial, num_pins, GPIO_HAL_PIN_DIR_OUTPUT);
		// init hal
	iniciar_serial_hal(linea_serie_drv_continuar_envio ,gestor_serial);
}

// se llama cada vez que se quiere procesar un caracter
void gestor_serial(void)
{
	int ch = getchar_serie();	// leer dato l�nea serie
	
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
				int32_t cmd = (trama_buffer[0] << 16) | (trama_buffer[1] << 8) | trama_buffer[2]; // almacenar comando
				
					// nueva partida
				if( trama_buffer[0] == 'N' && trama_buffer[1] == 'E' && trama_buffer[2] == 'W'){
						FIFO_encolar(ev_RX_SERIE, cmd);	// at�mico
				}
					// terminar partida
				else if( trama_buffer[0] == 'E' && trama_buffer[1] == 'N' && trama_buffer[2] == 'D'){
						FIFO_encolar(ev_RX_SERIE, cmd); // at�mico
				}
					// indica visualizar tablero
				else if( trama_buffer[0] == 'T' && trama_buffer[1] == 'A' && trama_buffer[2] == 'B'){
						FIFO_encolar(ev_RX_SERIE, cmd); // at�mico
				}
					// indica jugada introducida
				else if( trama_buffer[1] == '-'){
					if( trama_buffer[0] >= '1' && trama_buffer[0] <= '7' &&  trama_buffer[2] >= '1' && trama_buffer[2] <= '7' )
							FIFO_encolar(ev_RX_SERIE, cmd); // at�mico
				}
			}
			else{
				if( trama_len_buff < (MAX_LEN_TRAMA + 1)) {
					trama_buffer[trama_len_buff] = ch;	// a�adir cadena al buffer
					trama_len_buff++;
				}
				else{			
					gpio_hal_escribir(pin_inicial, num_pins, 1); // enceder led GPIO30 "GPIO_SERIE_ERROR"
					ESTADO = inicio; // secuencia de cadenas incongruente con tramas fuera del dominio
				}
			}
			break;
	}
}

// Inicializa estructura de gesti�n de mensajes
// por l�nea serie y env�ar primer caracter
void linea_serie_drv_enviar_array(char *buff)
{
	len_buff = strlen(buff);
	if(len_buff == 0)
		return; 
	
	// inicializar datos ( as� en caso de parte no opcional )
	buffer = buff;
	index = 0;
	
	// enviar primer caracter
	sendchar_serie(buffer[index]);
	index++;
}

// Transmite el resto de caracteres del mensaje
void linea_serie_drv_continuar_envio(void)
{
	// Verificar si hemos alcanzado el final del buffer
    if (index < len_buff) {
				size_t ind = index;
				sendchar_serie(buffer[ind]);
        index++; 	
    } else 
        FIFO_encolar(ev_TX_SERIE, 0); // transmisi�n del mensaje completo finalizada
}
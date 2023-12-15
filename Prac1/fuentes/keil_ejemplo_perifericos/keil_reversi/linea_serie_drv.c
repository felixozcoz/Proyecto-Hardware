#include "linea_serie_drv.h"

#include "linea_serie_hal.h"
#include "EVENTOS_T.h"
#include "cola_FIFO.h"
#include <string.h>
#include "cola_mensajes.h"
#include "tramas.h"

enum{ inicio, procesando};
static volatile unsigned int ESTADO = inicio;

// Buffer de trama
static char trama_buffer[MAX_LEN_TRAMA];
static size_t trama_len_buff;

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
			if ( ch == CARACTER_INICIO_TRAMA ){
				gpio_hal_escribir(pin_inicial, num_pins, 0); // desactivar GPIO30
				trama_len_buff = 0;
				ESTADO = procesando;
			}
			break;
		
		case procesando:
			
			if( ch == CARACTER_FIN_TRAMA )	// caracter fin de comando
			{
				FIFO_encolar(ev_RX_SERIE, tramaToInt(trama_buffer) ); // enviar comando mediante evento
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
	Mensaje_t tempMsg;
	
		// mensaje vacío
	if(! strlen(msg) )
		return; 
	
		// extraer contenido de parámetro si excede el tamaño del tipo Mensaje_t
	setMensaje(tempMsg, msg);
	
		// si se está tratando un mensaje: encolar
	if (index > -1)
		encolar_msg(tempMsg);
		// si cola de mensajes vacía: tratar
	else if( estaVacia_msg() || index == -1){
			// inicializar variables de mensaje
		setMensaje(msg_enviar, tempMsg);
		index = 0;
			// enviar primer caracter
		sendchar_serie(msg_enviar[index]);
		index++;
	}
	else
		encolar_msg(tempMsg);	
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
					desencolar_msg(msg_enviar);
					linea_serie_drv_enviar_array(msg_enviar);
				}
		}
}

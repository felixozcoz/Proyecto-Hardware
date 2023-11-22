#include "linea_serie_drv.h"

#define MAX_LEN_CMD 3

enum{ inicio, procesando, fin};
static volatile unsigned int ESTADO = inicio;
static volatile int buffer[3];
static volatile uint8_t len_buff = 0;

// se llama cada vez que se quiere procesar un caracter
void gestor_serial(uint32_t cadena)
{
	int ch = getchar();	// leer dato de U0RBR, previamente validado
	sendchar(ch);	
	
	switch(ESTADO){
		
		case inicio:
			if ( cadena == '#' ){
				len_buff = 0;
				ESTADO = procesando;
			}
			break;
		
		case procesando:
			
			if(cadena == '!')	// caracter fin de comando
			{
				if( buffer[0] == 'N' && buffer[1] == 'E' && buffer[2] == 'W'){
					// encolar evento de tipo new
				}
				else if( buffer[0] == 'E' && buffer[1] == 'N' && buffer[2] == 'D'){
					// encolar evento de tipo end
				}
				else if( buffer[1] == '-'){
					if( buffer[0] >= '1' && buffer[0] <= '7' &&  buffer[2] >= '1' && buffer[2] <= '7' ){
						// hacer jugada
					}
				}
			}
			else{
				if( len_buff < (MAX_LEN_CMD + 1))
				{
					buffer[len_buff] = cadena;	// añadir cadena al buffer
					len_buff++;
				}else
					ESTADO = inicio; // secuencia de cadenas incongruente con comandos permitidos
			}
				
			
		
			break;
		
		default:
			break;
	
	
	
}
	
#ifndef GPIO_HAL_H
#define GPIO_HAL_H

#include <inttypes.h>
#include <LPC210X.H> // LPC21XX Peripheral Registers

// Enumeración para la dirección del pin GPIO
typedef enum {
    GPIO_HAL_PIN_DIR_INPUT = 0,
    GPIO_HAL_PIN_DIR_OUTPUT = 1
} gpio_hal_pin_dir_t;

// Tipo de datos para la representación del pin del GPIO
typedef unsigned int GPIO_HAL_PIN_T;

// Función que permite emplear el GPIO y debe ser
// invocada antes de poder llamar al resto
// de funciones de la biblioteca
__inline void gpio_hal_iniciar(void)
{
	// IOCLR: Clear Output Register, controla el estado de salida de los pins
	IOCLR = 0xFFFFFFFF;
	IODIR = 0x00000000; // cambiar a INPUT por seguridad
}

// Función para configurar la dirección de los pines
//	gpio_inicial: indica el primer bit a leer
//	num_bits: indica cuántos bits se quieren leer
//	direccion: conjunto de direcciones E/S del GPIO (input/output)
__inline void gpio_hal_sentido(GPIO_HAL_PIN_T gpio_inicial, uint8_t num_bits, gpio_hal_pin_dir_t direccion) 
{			
	
	GPIO_HAL_PIN_T mask = (1 << num_bits) - 1; // crear máscara con tantos 1's como num_bits se van a modificar
	mask = mask << gpio_inicial;	// desplazar a la izq gpio_inicial bits
	
	if (direccion == GPIO_HAL_PIN_DIR_INPUT) {
		// Ejemplo:
		//  registro = 11111
		//  registro = registro & (~mask)
		//	registro = 11111    & (~11100)
		//	registro = 00011
		
		IODIR = IODIR & (~mask);
	
	} else {
		
		/// Ejemplo:
		//  registro = 00000
		//  registro = registro | mask
		//	registro = 00000    | 11100
		//	registro = 11100
		
		IODIR = IODIR | mask;
	}
		
}

// Función para leer los valores de los pines
//	gpio_inicial: indica el primer bit a leer
//	num_bits: omdoca cuántos bits se quieren leer
__inline uint32_t gpio_hal_leer(GPIO_HAL_PIN_T gpio_inicial, uint8_t num_bits) 
{
	// Extracción:
	//  1. Desplazamos el valor en IOPIN hacia la derecha un número de veces igual a gpio_inicial
	//	2. Creamos máscara con tantos 1's a la derecha como num_bits
	//	3. Hacemos AND 1 y 2
	
	return ( IOPIN >> gpio_inicial & (1 << num_bits) - 1) ; // 
}

// Función para escribir valores en los pines
__inline void gpio_hal_escribir(GPIO_HAL_PIN_T bit_inicial, uint8_t num_bits, uint32_t valor)
{
		// Máscara con el número de bits a escribir
    GPIO_HAL_PIN_T mask = (1 << num_bits) - 1;
	
		// Desplazar a la izq bit_inicial bits para limpiar los bits donde se va a escribir
		IOCLR = mask << bit_inicial;
  
		// Aplicamos máscara para quedarnos con los bits en los que se quiere escribir
		valor = valor & mask;
	
		// Desplazamos a la izq bit_inicial bits para escribir en los bits adecuados
		valor = valor << bit_inicial;
	
		// OR de los pines en IOSET y el valor que se quiere escribir
		IOSET = IOSET | valor;
}



#endif // GPIO_HAL_H

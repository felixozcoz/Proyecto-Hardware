#include "temporizador_drv.h"
#include "temporizador_hal.h"

#include <stdio.h>

// Variables para inserción de variables
static EVENTO_T evento = ev_EVENTO_VOID;

// Variable que guarda la función de callback
static void(*func_address)(EVENTO_T) = NULL ;

// ----------- API DRIVER ----------------

// inicializar un temporizador
void temporizador_drv_iniciar(void)
{
  temporizador0_hal_iniciar();
}

void temporizador_drv_empezar(void) 
{
	temporizador0_hal_empezar();
}

// Implementa la lectura del tiempo transcurrido
// y convierte el valor a microsegundos
uint64_t temporizador_drv_leer(void) {
	return (uint64_t)temporizador0_hal_leer();
}

// Implementa la detención del contador y el cálculo
// del tiempo transcurrido desde el último inicio
uint64_t temporizador_drv_parar(void) {
    uint64_t ticks = (uint64_t)temporizador0_hal_parar();
		uint64_t tiempo = ticks * temporizador0_hal_ticks2us;
	
		return (uint64_t)tiempo;
}

// TIMER 1 

// Programa el reloj para que encole un evento periódicamente 
// en la cola del planificador. El periodo se indica en ms
void temporizador_drv_reloj (uint32_t periodo, void (*funcion_callback)(), EVENTO_T ID_evento) 
{
	// guardamos el id del evento a encola en la próxima interrupción del reloj
	evento = ID_evento;
	func_address = funcion_callback;
	// inicializar parte dependiente del hardware
	temporizador_hal_reloj(periodo, temporizador_drv_callback_reloj);
}


uint64_t __swi(0) clock_get_us(void);
uint64_t __SWI_0 (void) {
	return (uint64_t)temporizador_drv_leer();
}

// Detener reloj (timer 1) y devolver count
uint64_t temporizador1_parar_drv(void){
	return temporizador1_hal_parar();
}



// Permite al hardware llamar a la función 
// de callback
void temporizador_drv_callback_reloj(void){
		// llama a función de callback proporcionada por el temporizador_drv_reloj
		func_address(evento);
}




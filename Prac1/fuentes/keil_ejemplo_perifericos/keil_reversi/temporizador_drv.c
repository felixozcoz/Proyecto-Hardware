#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include <LPC210X.H> // LPC21XX Peripheral Registers
#include "temporizador_drv.h"
#include "temporizador_hal.h"
#include "pulsacion.h"


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
	float ticks = (float)temporizador0_hal_leer();
	float tiempo = ticks/temporizador_hal_ticks2us;
	
	return (uint64_t)tiempo;
}

// Implementa la detención del contador y el cálculo
// del tiempo transcurrido desde el último inicio
uint64_t temporizador_drv_parar(void) {
    float ticks = (float)temporizador0_hal_parar();
		float tiempo = ticks/temporizador_hal_ticks2us;
	
		return (uint64_t)tiempo;
}

// TIMER 1 

// Programa el reloj para que encole un evento periódicamente 
// en la cola del planificador. El periodo se indica en ms
void temporizador_drv_reloj (uint32_t periodo, void (*funcion_encolar_evento)(), EVENTO_T ID_evento) 
{
	// inicializar parte dependiente del hardware
	temporizador_hal_reloj(periodo, funcion_encolar_evento);
	// y ahora que hago con el evento
}


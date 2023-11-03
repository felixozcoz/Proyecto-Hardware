#include "conecta_K_2023.h"
#include <stdio.h>
#include "Power_management.h"
#include "boton_eint0.h"
#include "temporizador_drv.h"
#include "planificador.h"
#include "tests.h"
#include "GPIO.h"


// MAIN: Hello world
int main() {
	// inicialiar timer, interrumpe cada 10ms
	temporizador_drv_reloj(1, FIFO_encolar, EVENTO_HELLO_WORLD);
	
	// inicializar planificador
	inicializar_cola_eventos();
	
	while(1);
}


// MAIN 
//int main (void) {
//	//inicializar sistema
//	//... practica 2
//	
//	//jugar
// 	conecta_K_jugar();
//	
//	while(1); //no hay S.O., no se retorna
//}

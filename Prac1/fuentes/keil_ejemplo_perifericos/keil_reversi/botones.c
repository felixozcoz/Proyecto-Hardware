#include "botones.h"
#include <LPC210X.H> // LPC21XX Peripheral Registers

#include "alarmas.h"


// Estados de los botones
typedef enum {
		PULSADO = 0,
		NO_PULSADO = 1,
} estado_t;

// estados de los botones
static  estado_t estado_eint1 = NO_PULSADO;
static  estado_t estado_eint2 = NO_PULSADO;

// funci�n para inicializar
// ambos botones
void inicializar_botones(void)
{
	eint1_iniciar_drv();
	eint2_iniciar_drv();
}

// EINT1 (External Interrupt 1)

// iniciar EINT1
void eint1_iniciar_drv (void)
{ eint1_iniciar_hal(); }
		
// leer pulsaci�n de EINT1
unsigned int eint1_leer_nueva_pulsacion_drv (void)
{ return eint1_leer_nueva_pulsacion_hal(); }

// nueva pulsacion de EINT1
void eint1_clear_nueva_pulsacion_drv (void)
{ eint1_clear_nueva_pulsacion_hal(); }

// leer cuenta de EINT1
unsigned int eint1_leer_cuenta_drv (void)
{ return eint1_leer_cuenta_hal(); }

// Reactivar interrupciones de EINT1
void eint1_reactivar_interrupciones_drv(void)
{ eint1_reactivar_interrupciones_hal(); }

// comprobar si EINT1 est� pulsado
unsigned int eint1_comprobar_pulsado_drv(void)
{ return eint1_comprobar_flag(); }


// M�quina de estados de bot�n EINT1
void eint1_gestionar_pulsacion(void)
{
	switch(estado_eint1){
		
		case PULSADO:
			// si flag no activo (valor 0)
			if ( ! eint1_comprobar_pulsado_drv() ) {
					eint1_clear_nueva_pulsacion_drv();
					estado_eint1 = NO_PULSADO;	// cambiar estado del boton
					eint1_reactivar_interrupciones_drv();
					// cancelar alarma
					alarma_activar(PULSACION, 0x00000000, 1);
			}
			break;
		case NO_PULSADO:
			// si flag activo (valor 1)
			if ( eint1_comprobar_pulsado_drv() ){
				estado_eint1 = PULSADO;
				// programar alarma peri�dica con retardo 100ms
				// TODO: recordar cambiar el retardo a 100ms
				alarma_activar(PULSACION, 0x8000000A, 1);
			}
			break;
		default:
				while(1); // se ha producido un error
	}
}




// EINT2 (External Interrupt 2)

// iniciar EINT2
void eint2_iniciar_drv (void)
{ eint2_iniciar_hal(); }

// leer pulsaci�n EINT2
unsigned int eint2_leer_nueva_pulsacion_drv( void)
{ return eint2_leer_nueva_pulsacion_hal(); }

// limpiar nueva pulsacion de EINT2
void eint2_clear_nueva_pulsacion_drv (void)
{ eint2_clear_nueva_pulsacion_hal(); }

// leer cuenta de EINT2
unsigned int eint2_leer_cuenta_drv (void)
{ return eint2_leer_cuenta_hal(); }

// Reactivar interrupciones de EINT2
void eint2_reactivar_interrupciones_drv(void)
{ eint1_reactivar_interrupciones_hal(); }

unsigned int eint2_comprobar_pulsado_drv(void)
{ return eint2_comprobar_flag(); }

// M�quina de estados de bot�n EINT2
void eint2_gestionar_pulsacion(void)
{
	switch(estado_eint2){
		
		case PULSADO:
			// si flag no activo (valor 0)
			if ( ! eint2_comprobar_pulsado_drv() ) {
					eint2_clear_nueva_pulsacion_drv();
					estado_eint2 = NO_PULSADO;	// cambiar estado del boton
					eint2_reactivar_interrupciones_drv();
					// cancelar alarma
					alarma_activar(PULSACION, 0x00000000, 2);
			}
			break;
		case NO_PULSADO:
			// si flag activo (valor 1)
			if ( eint2_comprobar_pulsado_drv() ){
				estado_eint2 = PULSADO;
				// programar alarma peri�dica con retardo 100ms
				// TODO: recordar cambiar el retardo a 100ms
				alarma_activar(PULSACION, 0x8000000A, 2);
			}
			break;
		default:
				while(1); // se ha producido un error
	}
}

// Funci�n para gesti�n de pulsaci�n
void pulsacion_detectada(const uint8_t id_boton)
{
		FIFO_encolar(PULSACION, id_boton);
}
	

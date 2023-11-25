#include <LPC210X.H> // LPC21XX Peripheral Registers

#include "botones.h"
#include "alarmas.h"
#include "int_external_hal.h"
#include "cola_FIFO.h"

// Definir tipo para 
// estados de los botones
typedef enum {
		PULSADO = 0,
		NO_PULSADO = 1,
} estado_t;

// Estados de los botones
static  estado_t estado_eint1 = NO_PULSADO;
static  estado_t estado_eint2 = NO_PULSADO;


// **** Funciones ****

void inicializar_botones(void)
{
	eint1_iniciar_drv( pulsacion_detectada );
	eint2_iniciar_drv( pulsacion_detectada );
}

// EINT1 (External Interrupt 1)

// iniciar EINT1
void eint1_iniciar_drv (void(*callback)(uint8_t))
{ eint1_iniciar_hal( callback ); }
		
// leer pulsación de EINT1
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

// comprobar si EINT1 está pulsado
unsigned int eint1_comprobar_pulsado_drv(void)
{ return eint1_comprobar_flag(); }


// Máquina de estados finita de botón EINT1
void eint1_gestionar_pulsacion(void)
{
	switch(estado_eint1){
		
		case PULSADO:
			if ( ! eint1_comprobar_pulsado_drv() ) 
			{
					eint1_clear_nueva_pulsacion_drv();
				
					estado_eint1 = NO_PULSADO;	// cambiar estado del boton
					eint1_reactivar_interrupciones_drv();	
				
					alarma_activar(PULSACION, CANCELAR_ALARMA, BOTON_1); // cancelar alarma revisión
					FIFO_encolar(DESPULSACION, BOTON_1); // indicar despulsado
			}
			break;
			
		case NO_PULSADO:
			if ( eint1_comprobar_pulsado_drv() )
			{
				estado_eint1 = PULSADO;
				// TODO: recordar cambiar el retardo a 100ms
				alarma_activar(PULSACION, 0x8000000A, BOTON_1); // alarma periódica revisión cada 100ms
			}
			break;
			
		default:
				while(1); // se ha producido un error
	}
}




// EINT2 (External Interrupt 2)

// iniciar EINT2
void eint2_iniciar_drv (void(*callback)(uint8_t))
{ eint2_iniciar_hal( callback ); }

// leer pulsación EINT2
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
{ eint2_reactivar_interrupciones_hal(); }

unsigned int eint2_comprobar_pulsado_drv(void)
{ return eint2_comprobar_flag(); }

// Máquina de estados de botón EINT2
void eint2_gestionar_pulsacion(void)
{
	switch(estado_eint2){
		
		case PULSADO:
			if ( ! eint2_comprobar_pulsado_drv() ) 
			{
					eint2_clear_nueva_pulsacion_drv();
				
					estado_eint2 = NO_PULSADO;	
					eint2_reactivar_interrupciones_drv();

				alarma_activar(PULSACION, CANCELAR_ALARMA, BOTON_2); // cancelar alarma revisión
				FIFO_encolar(DESPULSACION, BOTON_2); // indicar despulsado EINT2
			}
			break;
			
		case NO_PULSADO:
			if ( eint2_comprobar_pulsado_drv() )
			{
				estado_eint2 = PULSADO;
				// TODO: recordar cambiar el retardo a 100ms
				alarma_activar(PULSACION, 0x8000000A, BOTON_2);	// periódica 100ms revisión
			}
			break;
			
		default:
				while(1); // se ha producido un error
	}
}

// Función para gestión de pulsación
void pulsacion_detectada(const uint8_t id_boton)
{
		FIFO_encolar(PULSACION, id_boton);
}
	

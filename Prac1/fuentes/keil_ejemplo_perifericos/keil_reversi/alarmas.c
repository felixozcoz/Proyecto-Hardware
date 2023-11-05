#include <stdio.h>
#include <limits.h>
#include "alarmas.h"


// Alarmas
static alarma_t alarmas[ALARMAS_MAX];


	// Indica el estado de la alarma
const bool OCUPADA = false;
const bool LIBRE = true;
	// Ctes para facilitar legibilidad
const uint32_t DESACTIVAR_ALARMA = 0;
const int8_t ALARMAS_OCUPADAS = -1;


// Funciones auxiliares
int8_t buscar_alarma_libre(void);

// (Funci�n auxiliar)
bool is_periodic(uint32_t retardo);

// (Funci�n auxiliar)
void cancelar_alarma(EVENTO_T _ID_evento);

// (Funci�n auxiliar)
bool cumplido_retardo(uint8_t id_alarma);


// ---- API ----


// Reset alarmas
void alarma_inicializar(void)
{
	uint8_t i;
	for(i = 0 ; i < ALARMAS_MAX; i++){
			alarmas[i].libre = LIBRE;
	}
	
	// el resto de par�metro son inicializados
	// cuando se programa la alarma
}


// Inicializar nueva alarma
void alarma_activar(EVENTO_T _ID_evento, uint32_t _retardo, uint32_t _auxData)
{
		int8_t id; // guarda id alarma
	
		if( _retardo == DESACTIVAR_ALARMA){
				cancelar_alarma(_ID_evento);
				return;
		}
		
		// devuelve el id de la alarma si la hay, sino -1
		id = buscar_alarma_libre();
		
		if (id != ALARMAS_OCUPADAS){
				// set evento
			alarmas[id].ID_evento = _ID_evento;
			alarmas[id].auxData = _auxData;
				// set tiempo de duraci�n y contador
			alarmas[id].inicio = 0; // SEGURO ???
			alarmas[id].retardo = _retardo;
			alarmas[id].periodica = is_periodic(_retardo);
				// set estado de la alarma
			alarmas[id].libre = OCUPADA;
		}
}


// Revisar alarmas
void alarma_tratar_evento(void)
{
	uint8_t i;
	for( i = 0; i < ALARMAS_MAX; i++){
		
		if( cumplido_retardo(i) ){
			// tratar seg�n si es peri�dica
			if( alarmas[i].periodica ){
					alarmas[i].inicio = 0;  // reset tiempo 
			} 
			else {
					alarmas[i].libre = LIBRE; // liberar alarma
			}
			
			// encolar evento de la alarma i-�sima
			FIFO_encolar(alarmas[i].ID_evento, alarmas[i].auxData);
		}
	}
}


// ---- FUNCIONES AUXILIARES ----


// buscar_alarma_libre
//
// (Funci�n auxiliar)
//
// Busca la primera alarma libre, si existe
// devuelve la id de su componente en 'alarmas'.
// 	Si no hay ninguna libre
// genera un evento ALARMA_OVERFLOW
// y devuelve -1
// 
int8_t buscar_alarma_libre(void)
{
	uint8_t i;
	for(i = 0; i < ALARMAS_MAX; i++)
		if (! alarmas[i].libre) return i;
	
	// no hay alarmas libres
	// generar evento
	FIFO_encolar(ALARMA_OVERFLOW, 0);
	
	return ALARMAS_OCUPADAS;
	// nota: cu�ndo lo gestionar� el planificador??
	// y si para cuando lo gestione ya hay una libre
}
	
// is_periodic
//
// (Funci�n auxiliar)
//
// Par�metros:
//	retardo: tiempo en ms. El bit de m�s peso 
//  indica si la alarma es peri�dica
//
// Devuelve verdadero si el bit de m�s peso es 1
// en cualquier otro caso devuelve false
// 
__inline bool is_periodic(const uint32_t _retardo)
{
	return (_retardo >> (sizeof(uint32_t) * CHAR_BIT - 1)) ? true : false;
}


// cancelar_alarma
//
// (Funci�n auxiliar)
//
// Par�metros:
//	_ID_evento: indica el evento que gestiona la alarma
//
// Comprueba cu�l de las alarmas gestiona ese evento
// e indica que ahora est� libre
void cancelar_alarma(const EVENTO_T _ID_evento)
{
	uint8_t i;
	for(i = 0 ; i < ALARMAS_MAX; i++){
		if(alarmas[i].ID_evento == _ID_evento) {
				alarmas[i].libre = LIBRE;
		}
	}
}


// cumplido_retardo
//
// (Funci�n auxiliar)
//
// Par�metros:
//	id_alarma: componente de alarmas a revisar
//
// Devueve verdadero si ha cumplido el tiempo de la alarma
// y false en cualquier otro caso
__inline bool cumplido_retardo(const uint8_t id_alarma)
{
		return true ? alarmas[id_alarma].inicio >= alarmas[id_alarma].retardo : false ;
}



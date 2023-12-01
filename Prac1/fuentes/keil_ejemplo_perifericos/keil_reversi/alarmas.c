#include <stdio.h>
#include <limits.h>
#include "alarmas.h"

#include "cola_FIFO.h"

// Alarmas
static alarma_t alarmas[ALARMAS_MAX];


	// Indica el estado de la alarma
const bool DESACTIVADA = false;
const bool ACTIVA = true;
	// Ctes para facilitar legibilidad
const uint32_t DESACTIVAR_ALARMA = 0;
const int8_t ALARMAS_OCUPADAS = -1;


// Funciones auxiliares
int8_t buscar_alarma_libre(const EVENTO_T _ID_evento, const uint32_t _retardo, const uint32_t _auxData);

// (Función auxiliar)
bool is_periodic(uint32_t retardo);

// (Función auxiliar)
void cancelar_alarma(EVENTO_T _ID_evento);

// (Función auxiliar)
bool cumplido_retardo(uint8_t id_alarma);


// ---- API ----


// Reset alarmas
void alarma_inicializar(void)
{
	uint8_t i;
	for(i = 0 ; i < ALARMAS_MAX; i++){
							// set evento
			alarmas[i].ID_evento = ev_EVENTO_VOID;
			alarmas[i].auxData = 0;
				// set tiempo de duración y contador
			alarmas[i].inicio = 0; 
			alarmas[i].retardo = 0;
			alarmas[i].periodica = false;
				// set estado de la alarma
			alarmas[i].activa = DESACTIVADA;
	}
	
	// el resto de parámetro son inicializados
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
		id = buscar_alarma_libre(_ID_evento, _retardo, _auxData);
		
		if (id != ALARMAS_OCUPADAS){
				// set evento
			alarmas[id].ID_evento = _ID_evento;
			alarmas[id].auxData = _auxData;
				// set tiempo de duración y contador
			alarmas[id].inicio = 0; 
			alarmas[id].retardo = _retardo;
			alarmas[id].periodica = is_periodic(_retardo);
				// set estado de la alarma
			alarmas[id].activa = ACTIVA;
		}
}


// Revisar alarmas
// El planificador ejecuta esta función
// cada 'periodo_timer1' en ms
void alarma_tratar_evento(const uint32_t periodo_timer1)
{
	uint8_t i;
	for( i = 0; i < ALARMAS_MAX; i++){
		
		if( alarmas[i].activa ) {
			// sumar retardo
			alarmas[i].inicio += periodo_timer1;
			
			if( cumplido_retardo(i) ){
				// tratar según si es periódica
				if( alarmas[i].periodica )  
					alarmas[i].inicio = 0;  // reset tiempo 
				else  
					alarmas[i].activa = DESACTIVADA; // liberar alarma
				
				// encolar evento de la alarma i-ésima
				FIFO_encolar(alarmas[i].ID_evento, alarmas[i].auxData);
			} 
		}
	}
}


// ---- FUNCIONES AUXILIARES ----

// buscar_alarma_libre
//
// (Función auxiliar)
//
// Busca la primera alarma que administre el 
// evento _ID_evento ó la primera alarma libre (en este orden)
// En cualquiera de los dos casos
// devuelve la índice de su componente en 'alarmas'.
// 	Si no hay ninguna libre
// genera un evento ALARMA_OVERFLOW
// y devuelve ALARMAS_OCUPADAS (valor -1)
// 
int8_t buscar_alarma_libre(const EVENTO_T _ID_evento, const uint32_t _retardo, const uint32_t _auxData)
{
	uint64_t i;
	uint64_t primera_libre = ALARMAS_MAX;
	
	for(i = 0; i < ALARMAS_MAX; i++){
		// si existe una alarma con _ID_evento se reprograma
		if( alarmas[i].ID_evento == _ID_evento && alarmas[i].auxData == _auxData) return i; 
		// si no se devuelve la componente de la primera libre
		else if ( alarmas[i].activa == DESACTIVADA && i < primera_libre ) primera_libre = i;
	}
	
	if ( primera_libre != ALARMAS_MAX)
			return primera_libre;
	
	// no hay alarmas libres
	// generar evento
	FIFO_encolar(ev_ALARMAS_OVERFLOW, 0);
	
	return ALARMAS_OCUPADAS; // return -1;
	// nota: cuándo lo gestionará el planificador??
	// y si para cuando lo gestione ya hay una libre
}
	
// is_periodic
//
// (Función auxiliar)
//
// Parámetros:
//	retardo: tiempo en ms. El bit de más peso 
//  indica si la alarma es periódica
//
// Devuelve verdadero si el bit de más peso es 1
// en cualquier otro caso devuelve false
// 
__inline bool is_periodic(const uint32_t _retardo)
{
	return (_retardo >> (sizeof(uint32_t) * CHAR_BIT - 1)) ? true : false;
}


// cancelar_alarma
//
// (Función auxiliar)
//
// Parámetros:
//	_ID_evento: indica el evento que gestiona la alarma
//
// Comprueba cuál de las alarmas gestiona ese evento
// e indica que ahora está activa
void cancelar_alarma(const EVENTO_T _ID_evento)
{
	uint8_t i;
	for(i = 0 ; i < ALARMAS_MAX; i++){
		if(alarmas[i].ID_evento == _ID_evento) {
				alarmas[i].activa = DESACTIVADA;
		}
	}
}


// cumplido_retardo
//
// (Función auxiliar)
//
// Parámetros:
//	id_alarma: componente de alarmas a revisar
//
// Devueve verdadero si ha cumplido el tiempo de la alarma
// y false en cualquier otro caso
__inline bool cumplido_retardo(const uint8_t id_alarma)
{
		// con la operación AND se suprime el bit de periodicidad del retardo
		return alarmas[id_alarma].inicio >= (alarmas[id_alarma].retardo & 0x7fffffff);
}



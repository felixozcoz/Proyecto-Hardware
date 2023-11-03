#include "alarmas.h"

// Alarmas
static alarma_t alarmas[ALARMAS_MAX];


	// Indica el estado de la alarma
const bool OCUPADA = true;
const bool LIBRE = false;
	// Ctes para facilitar legibilidad
const uint32_t DESACTIVAR_ALARMA = 0;
const int8_t ALARMAS_OCUPADAS = -1;


// Funciones auxiliares
uint8_t buscar_alarma_libre(void);

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
	for(int i = 0 ; i < ALARMAS_MAX; i++)
			alarmas[i].status = LIBRE;
	
	// el resto de parámetro son inicializados
	// cuando se programa la alarma
}


// Inicializar nueva alarma
void alarma_activar(EVENTO_T _ID_evento, uint32_t _retardo, uint32_t _auxData)
{
		if( _retardo == DESACTIVAR_ALARMA){
				cancelar_alarma(_ID_evento);
				return;
		}
		
		// buscar alarma disponible, devuelve id si la hay
		uint8_t id = buscar_alarma_libre();
		
		// si encontrada alguna alarma libre
		if (id != ALARMAS_OCUPADAS){
			alarmas[id].inicio = 0; // SEGURO ???
			alarmas[id].ID_evento = _ID_evento;
			alarmas[id].auxData = _auxData;
			alarmas[id].retardo = _retardo;
			alarmas[id].status = OCUPADA;
			alarmas[id].periodica = is_periodic(_retardo);
		}
}


// Revisar alarmas
void alarma_tratar_evento(void)
{
	for(uint8_t i = 0; i < ALARMAS_MAX; i++){
		
		if( cumplido_retardo(i) ){
			// tratar según si es periódica
			if( alarmas[i].periodica ){
					alarmas[i].inicio = 0;  // reset tiempo 
			} 
			else {
					alarmas[i].status = LIBRE; // liberar alarma
			}
			
			// encolar evento
			FIFO_encolar(alarmas[i].ID_evento, alarmas[i].auxData);
		}
	}
}


// ---- FUNCIONES AUXILIARES ----


// buscar_alarma_libre
//
// (Función auxiliar)
//
// Busca la primera alarma libre, si existe
// devuelve la id de su componente en 'alarmas'.
// 	Si no hay ninguna libre
// genera un evento ALARMA_OVERFLOW
// y devuelve -1
// 
uint8_t buscar_alarma_libre(void)
{
	for( uint8_t i = 0; i < ALARMAS_MAX; i++)
		if (! alarmas[i].status) return i;
	
	// no hay alarmas libres: generar evento
	// ALARMA_OVERFLOW
	FIFO_encolar(ALARMA_OVERFLOW, 0);
	
	return ALARMAS_OCUPADAS;
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
bool is_periodic(const uint32_t retardo)
{
	uint8_t num_bits = sizeof(uint32_t) * __CHAR_BIT__;
	uint8_t mask = 1 << (num_bits -1); 
	
	return retardo & mask;
}


// cancelar_alarma
//
// (Función auxiliar)
//
// Parámetros:
//	_ID_evento: indica el evento que gestiona la alarma
//
// Comprueba cuál de las alarmas gestiona ese evento
// e indica que ahora está libre
void cancelar_alarma(EVENTO_T _ID_evento)
{
	for(int i = 0 ; i < ALARMAS_MAX; i++){
		if(alarmas[i].ID_evento == _ID_evento) {
				alarmas[i].status = LIBRE;
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
bool cumplido_retardo(uint8_t id_alarma)
{
		if(alarmas[id_alarma].inicio >= alarmas[id_alarma].periodica)
				return true;
		
		return false;
}








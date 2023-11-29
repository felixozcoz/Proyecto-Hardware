#include <LPC210x.h>

#include "juego.h"
#include "cola_FIFO.h"
#include "temporizador_drv.h"
#include "botones.h"
#include "tablero.h"
#include "conecta_K_2023.h"
#include "linea_serie_drv.h"
#include "Mensaje_t.h"
#include "celda.h"

// Contabiliza las pulsaciones de los botone
// pulsar EINT1 incrementa en una unidad y
// pulsar EINT2 decrementa en una unidad
// static int __attribute__((unused)) cuenta;

// Guarda el tiempo transcurrido
// entre las dos últimas pulsaciones
// static unsigned int	__attribute__((unused)) intervalo; // retirar el "__attribute__((unused))" cuando se utilice intervalo

// Ultima pulsación
// static unsigned int __attribute__((unused)) last_press = 0;


// Información juego
static TABLERO tablero;
static uint8_t pantalla[NUM_FILAS + 1][NUM_COLUMNAS + 1]; // visualización en memoria


// Función auxiliar
void imprimir_tablero_linea_serie(void);

// Inicializa el tablero del juego conecta_k
void inicializar_juego(uint8_t tab_input[NUM_FILAS][NUM_COLUMNAS]){
	tablero_inicializar(&tablero);
	conecta_K_cargar_tablero(&tablero, tab_input);
}


void juego_tratar_evento(const EVENTO_T ID_evento, const uint32_t auxData){
//	unsigned int now = clock_get_us(); // en ms
//	intervalo = (now - last_press);
//	last_press = now;
//	
//	if(auxData == BOTON_1)	// EINT1
//		cuenta ++;
//	else	// EINT2
//		cuenta--;
//	
//	FIFO_encolar(ev_VISUALIZAR_CUENTA, cuenta); // visualizar cuenta en GPIO
	
	// recibido ev_RX_SERIE
	if (ID_evento == ev_RX_SERIE){
			conecta_K_visualizar_tablero(&tablero, pantalla);
			imprimir_tablero_linea_serie();
	}
}


void imprimir_tablero_linea_serie(void)
{
	Mensaje_t msg;
	
	int offset = 0;
	size_t i,j;
	sprintf(msg + offset, "-|");
	offset+=2;
		// imprimir fila indicadora de columnas
  for (j = 1; j <= NUM_COLUMNAS; j++) {
     offset += sprintf(msg + offset, "%d|", j);
  }
	offset += sprintf(msg + offset, "\n");
	
  for (i = 1; i <= NUM_FILAS; i++) {
     offset += sprintf(msg + offset, "%d|", i);
     for (j = 0; j < NUM_COLUMNAS; j++) {
							// leer valor de tablero
						CELDA celda = tablero_leer_celda(&tablero, i-1, j);
						celda = celda_color(celda);
							// añadir caracteres según valor
            if (celda == 0)	
                offset += sprintf(msg + offset, " |");
            else if (celda == 1)
                offset += sprintf(msg + offset, "B|");
            else if (celda == 2)
                offset += sprintf(msg + offset, "N|");
           
        }
        offset += sprintf(msg + offset, "\n");
    }
		
		linea_serie_drv_enviar_array(msg);
}

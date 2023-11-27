#ifndef WD_H
#define WD_H

#include <inttypes.h>

// Inicializa el módulo watchdog
void WD_hal_inicializar(uint32_t sec);

// "Alimentar" Watchdog. Reinicia
// la cuenta del WD si no ha terminado.
void WD_hal_feed(void);

// WD_hal_test
//
// Parámetros:
//	sec: tiempo en segundos de la cuenta del watchdog
//
// Test unitario del módulo watchdog
void WD_hal_test(uint32_t sec);


#endif // WD_H

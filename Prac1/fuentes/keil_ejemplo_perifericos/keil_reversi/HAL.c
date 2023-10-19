#include <stdint.h>
#include <stdio.h>
#include <time.h>



// Definición de la constante de conversión de ticks a microsegundos
const uint64_t temporizador_hal_ticks2us = 1000000;

// Variables globales para el contador





// Función que programa un contador para que pueda ser utilizado
void temporizador_hal_iniciar() {
    if (clock_gettime(CLOCK_MONOTONIC, &start_time) == -1) {
        perror("Error al iniciar el contador");
    } else {
        contador_iniciado = 1;
    }
}

// Función para empezar la cuenta del contador de forma indefinida
void temporizador_hal_empezar() {
    if (contador_iniciado) {
        if (clock_gettime(CLOCK_MONOTONIC, &start_time) == -1) {
            perror("Error al empezar el contador");
        }
    } else {
        printf("El contador no ha sido iniciado. Llama a temporizador_hal_iniciar primero.\n");
    }
}

// Función que lee el tiempo que lleva 
// contando el contador desde la última vez que se ejecutó 
// temporizador_hal_empezar  y lo devuelve en ticks.
uint64_t temporizador_hal_leer() {
    if (contador_iniciado) {
        if (clock_gettime(CLOCK_MONOTONIC, &stop_time) == -1) {
            perror("Error al leer el contador");
            return 0;
        }

        uint64_t elapsed_time_ns = (stop_time.tv_sec - start_time.tv_sec) * 1000000000 + (stop_time.tv_nsec - start_time.tv_nsec);
        return elapsed_time_ns / 100;
    } else {
        printf("El contador no ha sido iniciado. Llama a temporizador_hal_iniciar primero.\n");
        return 0;
    }
}

// Función para detener el contador y devolver el tiempo transcurrido en ticks
// desde el último temporizador_hal_empezar.
uint64_t temporizador_hal_parar() {
    if (contador_iniciado) {
        if (clock_gettime(CLOCK_MONOTONIC, &stop_time) == -1) {
            perror("Error al parar el contador");
            return 0;
        }

        uint64_t elapsed_time_ns = (stop_time.tv_sec - start_time.tv_sec) * 1000000000 + (stop_time.tv_nsec - start_time.tv_nsec);
        contador_iniciado = 0;
        return elapsed_time_ns / 100;
    } else {
        printf("El contador no ha sido iniciado. Llama a temporizador_hal_iniciar primero.\n");
        return 0;
    }
	}

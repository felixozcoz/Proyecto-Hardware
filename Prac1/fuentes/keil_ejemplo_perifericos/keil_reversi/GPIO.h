// gpio.h - Archivo de cabecera de la biblioteca GPIO

#ifndef GPIO_H
#define GPIO_H

// Enumeraci�n para la direcci�n del pin GPIO
typedef enum {
    GPIO_HAL_PIN_DIR_INPUT,
    GPIO_HAL_PIN_DIR_OUTPUT
} gpio_hal_pin_dir_t;

// Tipo de datos para la representaci�n del pin del GPIO
typedef unsigned int GPIO_HAL_PIN_T;

// Funci�n para inicializar el GPIO
inline void gpio_hal_iniciar() {
    // C�digo de inicializaci�n del GPIO
}

// Funci�n para configurar la direcci�n de los pines
inline void gpio_hal_sentido(GPIO_HAL_PIN_T gpio_inicial, uint8_t num_bits, gpio_hal_pin_dir_t direccion) {
    // C�digo para configurar la direcci�n de los pines
}

// Funci�n para leer los valores de los pines
inline uint32_t gpio_hal_leer(GPIO_HAL_PIN_T gpio_inicial, uint8_t num_bits) {
    // C�digo para leer los valores de los pines
    // Devuelve el valor le�do
}

// Funci�n para escribir valores en los pines
inline void gpio_hal_escribir(GPIO_HAL_PIN_T bit_inicial, uint8_t num_bits, uint32_t valor) {
    // C�digo para escribir valores en los pines
}

#endif // GPIO_H

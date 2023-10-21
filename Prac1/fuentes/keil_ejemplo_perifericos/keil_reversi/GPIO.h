// gpio.h - Archivo de cabecera de la biblioteca GPIO

#ifndef GPIO_H
#define GPIO_H

// Enumeración para la dirección del pin GPIO
typedef enum {
    GPIO_HAL_PIN_DIR_INPUT,
    GPIO_HAL_PIN_DIR_OUTPUT
} gpio_hal_pin_dir_t;

// Tipo de datos para la representación del pin del GPIO
typedef unsigned int GPIO_HAL_PIN_T;

// Función para inicializar el GPIO
inline void gpio_hal_iniciar() {
    // Código de inicialización del GPIO
}

// Función para configurar la dirección de los pines
inline void gpio_hal_sentido(GPIO_HAL_PIN_T gpio_inicial, uint8_t num_bits, gpio_hal_pin_dir_t direccion) {
    // Código para configurar la dirección de los pines
}

// Función para leer los valores de los pines
inline uint32_t gpio_hal_leer(GPIO_HAL_PIN_T gpio_inicial, uint8_t num_bits) {
    // Código para leer los valores de los pines
    // Devuelve el valor leído
}

// Función para escribir valores en los pines
inline void gpio_hal_escribir(GPIO_HAL_PIN_T bit_inicial, uint8_t num_bits, uint32_t valor) {
    // Código para escribir valores en los pines
}

#endif // GPIO_H

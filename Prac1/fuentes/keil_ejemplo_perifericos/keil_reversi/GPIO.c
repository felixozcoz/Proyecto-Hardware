// Funciónque permite emplear el GPIO y debe ser
// invocada antes de poder llamar al resto
// de funciones de la biblioteca
inline void gpio_hal_iniciar() {
}

// Función para configurar la dirección de los pines
inline void gpio_hal_sentido(GPIO_HAL_PIN_T gpio_inicial, uint8_t num_bits, gpio_hal_pin_dir_t direccion) {
}

// Función para leer los valores de los piWnes
inline uint32_t gpio_hal_leer(GPIO_HAL_PIN_T gpio_inicial, uint8_t num_bits) {
    // Código para leer los valores de los pines
    // Devuelve el valor leído
}

// Función para escribir valores en los pines
inline void gpio_hal_escribir(GPIO_HAL_PIN_T bit_inicial, uint8_t num_bits, uint32_t valor) {
    // Código para escribir valores en los pines
}

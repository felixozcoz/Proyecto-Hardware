// Funci�nque permite emplear el GPIO y debe ser
// invocada antes de poder llamar al resto
// de funciones de la biblioteca
inline void gpio_hal_iniciar() {
}

// Funci�n para configurar la direcci�n de los pines
inline void gpio_hal_sentido(GPIO_HAL_PIN_T gpio_inicial, uint8_t num_bits, gpio_hal_pin_dir_t direccion) {
}

// Funci�n para leer los valores de los piWnes
inline uint32_t gpio_hal_leer(GPIO_HAL_PIN_T gpio_inicial, uint8_t num_bits) {
    // C�digo para leer los valores de los pines
    // Devuelve el valor le�do
}

// Funci�n para escribir valores en los pines
inline void gpio_hal_escribir(GPIO_HAL_PIN_T bit_inicial, uint8_t num_bits, uint32_t valor) {
    // C�digo para escribir valores en los pines
}

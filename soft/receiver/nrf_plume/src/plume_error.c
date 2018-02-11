
#include "conf.h"


void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name) {
    
    #ifdef DEBUG_NRF_USER
    
    ble_debug_assert_handler(error_code, line_num, p_file_name);
    #else
    
    nrf_gpio_pin_set(DEBUG_LED_PIN_NO);
    BUSY_WAIT_MS(1000); /* Wait for the user to notify the led and reset  */
    #endif
    // On assert, the system can only recover with a reset.
    NVIC_SystemReset();
}

void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name) {
    app_error_handler(DEAD_BEEF, line_num, p_file_name);
}


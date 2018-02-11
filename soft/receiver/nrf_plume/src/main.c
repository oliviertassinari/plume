/* Copyright (c) 2012 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

#include "conf.h"

static void power_manage(void) {
    uint32_t err_code = sd_app_evt_wait();
    APP_ERROR_CHECK(err_code);
}

static void leds_init(void) {
    nrf_gpio_cfg_output(CONNECTED_LED_PIN_NO);
}

void timers_init(void) {
  // Initialize timer module, making it use the scheduler
  APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_MAX_TIMERS, APP_TIMER_OP_QUEUE_SIZE, false);
}

static void services_init(void) {
    bas_init();
    ble_plume_init();
}

int main(void) {
    // Initialize

    leds_init();
    timers_init();
    gpiote_init();
    ble_stack_init();

    params_init();

    services_init();
    advertising_init();

    // Start execution
    advertising_start();
    uart_init();

    // Enter main loop
    for (;;) {
	uart_handler();
	power_manage();
    }
}

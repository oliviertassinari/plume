
#include "conf.h"
#define UART_CONFIGURED "Uart Configured.\r\n"

void uart_config(  uint8_t rts_pin_number,
		   uint8_t txd_pin_number,
		   uint8_t cts_pin_number,
		   uint8_t rxd_pin_number) {
    
    nrf_gpio_cfg_output(txd_pin_number);
    nrf_gpio_cfg_input(rxd_pin_number, NRF_GPIO_PIN_NOPULL); 
    NRF_UART0->PSELTXD = txd_pin_number;
    NRF_UART0->PSELRXD = rxd_pin_number;
    
    /* No hardware flow control */ 
    nrf_gpio_cfg_output(rts_pin_number);
    nrf_gpio_cfg_input(cts_pin_number, NRF_GPIO_PIN_PULLDOWN);
	
    NRF_UART0->PSELRTS = 0xFFFFFFFF;
    NRF_UART0->PSELCTS = 0xFFFFFFFF;
    NRF_UART0->CONFIG &= ~(UART_CONFIG_HWFC_Enabled << UART_CONFIG_HWFC_Pos);
    

    NRF_UART0->BAUDRATE         = (UART_BAUDRATE_BAUDRATE_Baud115200 << UART_BAUDRATE_BAUDRATE_Pos);
    NRF_UART0->ENABLE           = (UART_ENABLE_ENABLE_Enabled << UART_ENABLE_ENABLE_Pos);
    NRF_UART0->TASKS_STARTTX    = 1;
    NRF_UART0->TASKS_STARTRX    = 1;
    NRF_UART0->EVENTS_RXDRDY    = 0;
    NRF_UART0->EVENTS_TXDRDY    = 0;
}

void uart_init(void) {
    sd_nvic_DisableIRQ(UART0_IRQn);
    uart_config(RTS_PIN_NUMBER, TX_PIN_NUMBER, CTS_PIN_NUMBER, RX_PIN_NUMBER);
    //Interrupt on rxrdy
    NRF_UART0->INTENSET=(UART_INTENSET_RXDRDY_Enabled << UART_INTENSET_RXDRDY_Pos);
    
    uint32_t err_code = sd_nvic_SetPriority(UART0_IRQn, APP_IRQ_PRIORITY_LOW);
    if (err_code != NRF_SUCCESS) {
	APP_ERROR_CHECK(err_code);
    }
    
    err_code = sd_nvic_EnableIRQ(UART0_IRQn);
    if (err_code != NRF_SUCCESS) {
	APP_ERROR_CHECK(err_code);
    }
    
    BUSY_WAIT_MS(1); /* Wait for uart to be configured */
    UART_WRITE_DBG(UART_CONFIGURED);
}

void uart_putc (const uint8_t cr) {
    CRITICAL_REGION_ENTER();
    NRF_UART0->TXD = cr;
    while (NRF_UART0->EVENTS_TXDRDY != 1
	   && NRF_UART0->EVENTS_RXTO != 1);
    NRF_UART0->EVENTS_TXDRDY = 0;
    CRITICAL_REGION_EXIT();
}

void uart_write(const uint8_t *data, const uint16_t size) {
    for (int i = 0; i < size; i ++) {
	uart_putc(data[i]);
    }
}

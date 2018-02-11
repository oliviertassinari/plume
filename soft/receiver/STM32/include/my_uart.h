#ifndef MY_UART_H
#define MY_UART_H
#include "ch.h"
#include <stdint.h>

void uart_init(void);
void uart_putc(const uint8_t c);
uint8_t uart_getc(const systime_t timeout);
void uart_wait_ack(void);


typedef enum {
    uartFLUSH  = 0x7b,
    uartACK    = 0x7c,
    uartESCAPE = 0x7d,
    uartFRAME  = 0x7e,
} uart_control_t;

#define UART_ESCAPE 0x20

#endif

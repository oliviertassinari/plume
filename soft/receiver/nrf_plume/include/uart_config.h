
#ifndef UART_NRF_H__
#define UART_NRF_H__

void uart_init(void);

void uart_putc(const uint8_t c);
void uart_write(const uint8_t *data, const uint16_t size);

#define UART_WRITE_CONST(cst) uart_write((uint8_t*)cst, sizeof(cst))

//#define DEBUGG

#ifdef DEBUGG
#define UART_WRITE_DBG(cst) UART_WRITE_CONST(cst)
#else
#define UART_WRITE_DBG(cst)
#endif

#endif

#ifndef UART_HANDLER_H__
#define UART_HANDLER_H__

#define UART_UNESCAPE_CHAR 0x20
#define MAX_PKT_SIZE 256

typedef enum {
    uartFLUSH     = 0x7b,
    uartACK       = 0x7c,
    uartESCAPE    = 0x7d,
    uartFRAME     = 0x7e,
} uart_char_t;

#define UART_CHAR_TO_ESCAPE_BEGIN uartFLUSH
#define UART_CHAR_TO_ESCAPE_END   uartFRAME

typedef enum {
    uartRESET     = 0xff, /* Software reset   */
    uartCOILSDATA = 0x23, /* coils data       */
    uartCALIBDATA = 0x0c, /* calibration data */
    uartSMPR      = 0x0d, /* sampling rate    */
    uartSTART     = 0x0e, /* on switch        */
    uartOFF       = 0x0f, /* off switch       */
    uartSTANDBY   = 0x10, /* standby mode     */
} uart_msg_t;

typedef struct {
    uint16_t size;
    uint8_t data[MAX_PKT_SIZE];
} uart_packet_t;

/* Used in the main loop to handle the completely formed packet */
void uart_handler(void);

/* Used to transmit one byte as the protocol define it */
void uart_write_control(const uint8_t cr);

/* Same but with an array and a size */
void uart_write_packet(const uint8_t type, 
		       uint8_t * const content, 
		       const uint8_t size);

#endif

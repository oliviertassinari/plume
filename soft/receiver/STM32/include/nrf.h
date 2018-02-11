#ifndef NRF_H
#define NRF_H

#include "ch.h"
#include "hal.h"

#include "debug.h"

// signal an ack was received
void uart_signal_ack(void);

// Set serial port speed
void nrf_init(void);

// Puts char, string
void nrf_putc(const char c);

// FLush incoming chars
void nrf_flush(void);

// Reset mailboxes
void mbSendReset(void);
void mbReceiveReset(void);

// API Frame exchange
int nrf_get_frame(uint8_t *const s);
void nrf_send_frame(const uint8_t* const mess, const uint16_t frameLength);

#endif // NRF_H

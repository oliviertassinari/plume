#include "ch.h"
#include "hal.h"

#include "chprintf.h"
#include "debug.h"

#include <stdlib.h>
#include <string.h>

#include "nrf.h"
#include "my_uart.h"

#define ACK_TIMEOUT 10

/* Declaration of mailbox and event */
static WORKING_AREA(waMail_box_nrf_send, 2048);
static MAILBOX_DECL(mail_box_nrf_send, waMail_box_nrf_send, sizeof(waMail_box_nrf_send)/sizeof(msg_t));


/*
 * Declaration of function and other
 */
void nrf_putc(const char c) {
    uart_putc(c);
}

static char nrf_getc_unescaped(void) {
    // Get char in MB receive and eventually escape them
    uint8_t c = uart_getc(TIME_INFINITE);
    if (c == uartESCAPE) {
	c = uart_getc(TIME_INFINITE) ^ UART_ESCAPE;
    }
    return (uint8_t)c;
}

void nrf_flush(void) {
    nrf_putc(uartFLUSH);
}

int nrf_get_frame(uint8_t *const s) {
    msg_t c = 0;

    // Search for start of frame
    
    for (;;) {
	/*if (chMBFetch(&mail_box_nrf_receive, &c, TIME_INFINITE) == RDY_TIMEOUT)
	  return 0;*/
	c = uart_getc(TIME_INFINITE);
	if (c == uartFRAME) {
	    break;
	}
	// Impossible start of frame
    }
    // Get size
    size_t size = nrf_getc_unescaped();

    // Get payload
    for (size_t i = 0; i < size; i++) {
	// Store incoming char
	s[i] = nrf_getc_unescaped();
    }
    return size;
}

static void escape_if_needed (const uint8_t character) {
    if((character == uartFLUSH)  || 
       (character == uartACK)    || 
       (character == uartESCAPE) || 
       (character == uartFRAME)) {
	// Escape character
	chMBPost(&mail_box_nrf_send, uartESCAPE, TIME_INFINITE);
	chMBPost(&mail_box_nrf_send, (character ^ UART_ESCAPE), TIME_INFINITE);
    }
    else {
	// else send raw char
	chMBPost(&mail_box_nrf_send, character, TIME_INFINITE);
    }
}

void nrf_send_frame(const uint8_t *const mess, const uint16_t frameLength){
    chMBPost(&mail_box_nrf_send, uartFRAME, TIME_INFINITE);
    escape_if_needed(frameLength);
    // Store in MB begin of frame
    for(int i=0; i < frameLength; i++) {
	// If need to escape, then escape
	escape_if_needed(mess[i]);
    }
}

void mbSendReset(void) {
    chMBReset(&mail_box_nrf_send);
}

/*
 * Thread that send data to the nRF
 */
__attribute__((noreturn))
static msg_t sendThread(void *arg) {
    (void)arg;
    //  chRegSetThreadName("sendThread");
    msg_t c;

    while (TRUE) {
	if(chMBFetch(&mail_box_nrf_send, &c, TIME_INFINITE) == RDY_OK) {
	    uart_wait_ack();
	    //Wait ack from nRF
	    nrf_putc((uint8_t)c);
	}
    }
}


void nrf_init() {

    //initialize uart
    uart_init();

    // Create the thread that send data to the nRF
    static WORKING_AREA(waSendThread, 512);
    chThdCreateStatic(waSendThread, sizeof(waSendThread), NORMALPRIO, sendThread, NULL);
}

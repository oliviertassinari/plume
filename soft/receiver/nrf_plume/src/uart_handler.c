

#include "conf.h"
//RingFifo_t gUartFifo;
#define UARTFIFO_SIZE 1024
#define UARTFIFO_CTRL_SIZE 128
#define UARTBUFFER_SIZE 256
#define UART_PKT_SIZE 1

static RINGFIFO_DECL(uartFifo, UARTFIFO_SIZE, uint8_t);
static RINGFIFO_DECL(uartFifoCtrl, UARTFIFO_CTRL_SIZE, uint8_t);
static RINGFIFO_DECL(uartBuffer, UARTBUFFER_SIZE, uint8_t);

/* controls for the main loop handler */
static uart_packet_t current_packet;
static bool resend_pls = false;

/* controls for the irq */
static uint8_t is_new_pkt = false;
static uint8_t escape_next = false;
static uint16_t buffer_pkt_id = 0;
static uint16_t buffer_pkt_len = 0;

/** Uart_Packet Macros**/

static bool uart_has_valid_packet(void) {
    return !RINGFIFO_EMPTY(uartFifoCtrl);
}

static void uart_get_packet(uart_packet_t * const packet) {
    RINGFIFO_RD(uartFifoCtrl, packet->size);
    for (int i = 0; i < packet->size; i++) {
	RINGFIFO_RD(uartFifo, packet->data[i]);
    }
}

static uart_msg_t uart_peek_type_of_packet(void) {
    return uartFifo.data[uartFifo.mask & uartFifo.rdIdx];
}

#define CURRENT_PACKET (current_packet.data + 1),(current_packet.size - 1)

static void handle_coils_packets(void) {

    if (plume_coils_notify_ok()) {
	if (!resend_pls) {
	    uart_get_packet(&current_packet);
	}
	else {
	    resend_pls = false;
	}
	
	err_t err_code = 
	    ble_plume_coils_update(&m_plume, CURRENT_PACKET);
    	
	
	if (err_code != NRF_SUCCESS &&
	    err_code != BLE_ERROR_INVALID_CONN_HANDLE &&
	    err_code != NRF_ERROR_INVALID_STATE &&
	    err_code != BLE_ERROR_NO_TX_BUFFERS &&
	    err_code != NRF_ERROR_DATA_SIZE) {
	    APP_ERROR_CHECK(err_code);
	}

	resend_pls = err_code == BLE_ERROR_NO_TX_BUFFERS;
       
	UART_WRITE_DBG("sent\r\n");
	//uart_write(CURRENT_PACKET);
    }

}

/* handler for the main loop */
void uart_handler(void) {
    /* send an ack to say "I'm alive" */
    static uint32_t loop_cpt = 0;
    if ((loop_cpt++ & 0xff) == 0xff) {
	uart_putc(uartACK);
    }
    
    /* handle packets */
    if (resend_pls) {
	handle_coils_packets();
    }
    if (uart_has_valid_packet()) {
	switch (uart_peek_type_of_packet()) {
	case uartCOILSDATA:
	    handle_coils_packets();
	    return;
	case uartRESET:
	    uart_get_packet(&current_packet); /* Discard packet */
	    NVIC_SystemReset();
	    return;
	default:
	    // should not happen
    	    // discard packet
	    resend_pls = false;
	    return;
	}
    }
}

/* For uart transmission */
static void escape_if_needed(const uint8_t cr) {
    if (cr >= UART_CHAR_TO_ESCAPE_BEGIN && cr <= UART_CHAR_TO_ESCAPE_END) {
	uart_putc(uartESCAPE);
	uart_putc(cr ^ UART_UNESCAPE_CHAR);
    }
    else {
	uart_putc(cr);
    }
}

void uart_write_packet(const uint8_t type, uint8_t * const content, 
		       const uint8_t size) {
    uart_putc(uartFRAME);
    escape_if_needed(size + 1);
    escape_if_needed(type);
    for (int i = 0; i < size; i++) {
	escape_if_needed(content[i]);
    }
}

void uart_write_control(const uint8_t type) {
    uart_putc(uartFRAME);
    uart_putc(0x01);  /* size is one, and doesn't need to be escaped */
    escape_if_needed(type);
}

/** IRQ handling functions **/

static void uart_stop_frame(void) {
    is_new_pkt = false;
    while (!RINGFIFO_EMPTY(uartBuffer)) {
	uint8_t tmp;
	RINGFIFO_RD(uartBuffer, tmp);
	RINGFIFO_WR(uartFifo, tmp);
    }
    RINGFIFO_RESET(uartBuffer);
    RINGFIFO_WR(uartFifoCtrl, buffer_pkt_len);
}

#define LEAVE 0
#define CONTINUE 1
static uint32_t special_handler (const uint8_t cr) {
    switch (cr) {
    case uartESCAPE: 
	/* register the escape character */
	escape_next = true;
	return LEAVE;
    
    case uartFRAME:
	/* register the new frame */
	is_new_pkt = true;
	buffer_pkt_len = 0;
	buffer_pkt_id = 0;
	RINGFIFO_RESET(uartBuffer);
	return LEAVE;
    
    case uartFLUSH:
	is_new_pkt = false;
	buffer_pkt_len = 0;
	buffer_pkt_id = 0;
	RINGFIFO_RESET(uartBuffer);
	RINGFIFO_RESET(uartFifo);
	RINGFIFO_RESET(uartFifoCtrl);
	return LEAVE;
    }
    return CONTINUE;
}

/* handlers for the irq */
static void uart_putc_irq(const uint8_t cr) {
    NRF_UART0->TXD = cr;
    while (NRF_UART0->EVENTS_TXDRDY != 1
	   && NRF_UART0->EVENTS_RXTO != 1);
    NRF_UART0->EVENTS_TXDRDY = 0;
}

static void ack_handler(void) {
    /* Each byte received, send an ACK */
    uart_putc_irq(uartACK);
}

static void uart_irq_handler(const uint8_t in) {
    /** Ack handler **/
    ack_handler();
    //uart_putc_irq(in);

    /* handler for the protocol */
    uint8_t cr = in;
    /* If previous char was an escape character, handle it */
    if (escape_next) {
 	escape_next = false;
 	cr = cr ^ UART_UNESCAPE_CHAR;
    }
    else {
 
 	/* Handler every special character, if it is one */
 	if (special_handler(cr) == LEAVE) {
 	    return;
 	}
    }
 
    /* Handling packet frame */
    /* LEN_LSB | DATA ... */
    /*    0        n - 1  */
    if (is_new_pkt) {
 	if (buffer_pkt_id == 0) {
 	    buffer_pkt_len = cr;
 	}
 	else {
 	    if (buffer_pkt_id <= buffer_pkt_len) {
 		RINGFIFO_WR(uartBuffer, cr);
 	    }
 	    if (buffer_pkt_id == buffer_pkt_len) {
 		uart_stop_frame();
 	    }
 	}
 	buffer_pkt_id++;
    }
}


void UART0_IRQHandler(void) { 
    /* Handle RX interrupt */
    if (NRF_UART0->EVENTS_RXDRDY) {
	NRF_UART0->EVENTS_RXDRDY = 0;
	uart_irq_handler((uint8_t)NRF_UART0->RXD);
    }
}


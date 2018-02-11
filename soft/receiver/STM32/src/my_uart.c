#include "my_uart.h"
#include "ch.h"
#include "hal.h"
#include "debug.h"
#include <stdint.h>

static WORKING_AREA(waMail_box_uart_send, 512);
static MAILBOX_DECL(mail_box_uart_send, waMail_box_uart_send, sizeof(waMail_box_uart_send)/sizeof(msg_t));

static WORKING_AREA(waMail_box_uart_receive, 512);
static MAILBOX_DECL(mail_box_uart_receive, waMail_box_uart_receive, sizeof(waMail_box_uart_receive)/sizeof(msg_t));

static SEMAPHORE_DECL(sem_nrf_ack,6);
static void uart_signal_ack(void) {
    chSemSignalI(&sem_nrf_ack);
}

void uart_wait_ack(void) {
    chSemWait(&sem_nrf_ack);
}

CH_IRQ_HANDLER(Vector18C);
void Vector18C(void){
  msg_t received;
  msg_t to_send, err;
  CH_IRQ_PROLOGUE();
  chSysLockFromIsr();

  if(UART8->SR & USART_SR_RXNE){
      received = (msg_t)UART8->DR;
      if (received == uartACK) {
	  uart_signal_ack();
      }
      else {
	  chMBPostI(&mail_box_uart_receive, received);
      }
  }
  else if ((UART8->SR & USART_SR_TXE) && (UART8->CR1 & USART_CR1_TXEIE)) {
      err = chMBFetchI(&mail_box_uart_send, &to_send);
      if (err == RDY_OK) {
	  UART8->DR = (uint8_t)to_send;
      }
      else {
	  UART8->CR1 &= ~USART_CR1_TXEIE; /* mask interrupt */
      }
  } 
  chSysUnlockFromIsr();
  CH_IRQ_EPILOGUE();
}

void uart_init(void){
  //enable peripheral clock
  RCC->APB1ENR |= RCC_APB1ENR_UART8EN;
  // if Fclk = 144MHz, sets baudrate to 115200
  UART8->BRR = 0x271;
  UART8->CR2 = 0;
  UART8->CR3 = 0;
  UART8->CR1 = ((UART8->CR1&(1<<14))| 
		USART_CR1_OVER8     | //oversampling by 8 (modifies bdrate)
		USART_CR1_UE        |
				//USART_CR1_WAKE      | //
		USART_CR1_RXNEIE    | //enable interrupt on RX line
		USART_CR1_TE        |
		USART_CR1_RE        
		);

  NVIC_SetPriority(UART8_IRQn, 12);
  NVIC_EnableIRQ(UART8_IRQn);
}

void uart_putc(const uint8_t c) { //, systime_t timeout) {
    msg_t tmp_msg = 0x00ff & c;
    chMBPost(&mail_box_uart_send, tmp_msg, TIME_INFINITE);
    UART8->CR1 |= USART_CR1_TXEIE; /* unmask interrupt */ 
}

uint8_t uart_getc(const systime_t timeout){
  msg_t received;

  chMBFetch(&mail_box_uart_receive,&received,timeout);
  //debug("<<%x\r\n",received);
  return (uint8_t)received;
}


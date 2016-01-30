/*
 * Perform UART startup initialization.
 */
#ifndef UART_H
#define UART_H


#include "../sys/buffer.h"

typedef struct {
	buffer_t rx;
	buffer_t tx;
} buffer_rx_tx_t;


#define UART_PORT_COUNT					3
#define UART_PORTS						USART1,USART2,USART3

/* Initialize USART port and fifo buffers
 * !!! This function does not enable APB clock and configure appropriate ports !!! */
int uart_init(USART_TypeDef* port, uint32_t baudRate, buffer_rx_tx_t *buff);

int uart_putchar(USART_TypeDef* port,char c);
int uart_getchar(USART_TypeDef* port);


/*
 * USART IRQ Handler proc
 */
void uart_handle_irq(USART_TypeDef* port);

#endif

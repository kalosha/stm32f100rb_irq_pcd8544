#include <stdint.h>
#include <stdio.h>

#include "stm32f10x.h"
#include "uart.h"

static buffer_rx_tx_t* _uart_pbuf[UART_PORT_COUNT];
static USART_TypeDef* _uart_ports[UART_PORT_COUNT] = { UART_PORTS };

int _uart_port_id(USART_TypeDef* port) {
	int i;
	for (i = 0; i < UART_PORT_COUNT; i++) {
		if (_uart_ports[i] == port)
			return i;
	}
	return -1;
}

int uart_init(USART_TypeDef* port, uint32_t baudRate, buffer_rx_tx_t *buff) {
	USART_InitTypeDef USART_InitStruct;
	int id = _uart_port_id(port);
	if (id >= 0) {
		_uart_pbuf[id] = buff; // Setup port FIFO buffers

		USART_StructInit(&USART_InitStruct);
		USART_InitStruct.USART_BaudRate = baudRate;
		USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(port, &USART_InitStruct);
		USART_ITConfig(port, USART_IT_RXNE, ENABLE);
		USART_Cmd(port, ENABLE);
	}
	return id;
}

int uart_putchar(USART_TypeDef* port, char c) {
	buffer_rx_tx_t *buff;
	int id = _uart_port_id(port);
	buff = _uart_pbuf[id];
	if (id >= 0 && buff != NULL) {
		while (buf_wr_len(&buff->tx) == 0)
			// Check if there is free space in the transmit buffer
			;
		buf_write(&buff->tx, &c); //write data to buffer
		// Enable TX buffer Empty interrupt
		USART_ITConfig(port, USART_IT_TXE, ENABLE);
	}
	return id;
}

int uart_getchar(USART_TypeDef* port) {
	uint8_t data;
	buffer_rx_tx_t *buff;
	int id = _uart_port_id(port);
	buff = _uart_pbuf[id];
	if (id >= 0 && buff != NULL) {
		if (buf_read(&buff->rx, &data))
			return data;
		else
			return -1;
	}
	return -1;
}

/*Interrupt handling */
void uart_handle_irq(USART_TypeDef* port) {
	uint8_t data;
	buffer_rx_tx_t *buff;
	int id = _uart_port_id(port);
	buff = _uart_pbuf[id];
	if (id >= 0 && buff != NULL)

		if (port->SR & USART_FLAG_RXNE) { // Receive data
			data = USART_ReceiveData(port);
			buf_write(&buff->rx, &data);
			USART_ClearFlag(port, USART_FLAG_RXNE);
		}
	if (port->SR & USART_FLAG_TXE) { // Transmit buffer is empty
		if (buf_rd_len(&buff->tx) == 0) {
			USART_ITConfig(port, USART_IT_TXE, DISABLE);
		} else {
			buf_read(&buff->tx, &data);
			USART_SendData(port, data);
		}
		USART_ClearFlag(port, USART_FLAG_TXE);
	}
}

void USART1_IRQHandler(void) {
	uart_handle_irq(USART1);
}


void USART2_IRQHandler(void) {
	uart_handle_irq(USART2);
}

/*	void USART3_IRQHandler(void) {
 *		uart_handle_irq(USART3);
 *	}
 */

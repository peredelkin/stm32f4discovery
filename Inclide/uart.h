/*
 * uart.h
 *
 *  Created on: 13 мая 2020 г.
 *      Author: ruslan
 */

#ifndef INCLIDE_UART_H_
#define INCLIDE_UART_H_

#include "stm32f4xx.h"
#include <stdbool.h>

typedef struct {
	uint8_t read;
	uint8_t write;
	uint8_t count;
	uint8_t* buffer;
	__IO uint32_t* NDTR;
} usart_dma_t;

extern void USART_BaudRate_Set(USART_TypeDef* usart,uint32_t fpclk, uint32_t baud);

extern void usart_read(usart_dma_t* usart_dma,uint8_t *data,uint16_t count);
extern uint8_t usart_bytesAvailable(usart_dma_t* usart_dma);
extern void usart_readyRead(usart_dma_t* usart_dma);
extern void usart_dma_read_handler(usart_dma_t* usart_dma);

#endif /* INCLIDE_UART_H_ */

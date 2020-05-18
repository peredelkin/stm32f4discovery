/*
 * uart.c
 *
 *  Created on: 13 мая 2020 г.
 *      Author: ruslan
 */

#include "uart.h"

bool USART_CR1_read(USART_TypeDef* usart,uint16_t mask) {
    if (usart->CR1 & mask) return 1;
    else return 0;
}

bool USART_OVER8_read(USART_TypeDef* usart) {
    return USART_CR1_read(usart,USART_CR1_OVER8);
}

void USART_BRR_set(USART_TypeDef* usart,uint16_t mantissa, uint16_t fraction) {
    usart->BRR = ((uint16_t) (mantissa << 4) & (uint16_t) USART_BRR_DIV_Mantissa) |
    		((uint16_t) fraction & (uint16_t) USART_BRR_DIV_Fraction);
}

void USART_BaudRate_Set(USART_TypeDef* usart,uint32_t fpclk, uint32_t baud) {
    uint32_t over8 = (uint32_t) USART_OVER8_read(usart);
    uint32_t mantissa = (fpclk / ((8 * (2 - over8)) * baud));
    uint16_t divider = (uint16_t) (fpclk / baud);
    uint16_t fraction = (uint16_t) (divider - (uint16_t) (mantissa << 4));
    if (over8) {
    	USART_BRR_set(usart,mantissa, (fraction & (uint16_t) 0x07));
    } else {
    	USART_BRR_set(usart,mantissa, fraction);
    }
}

void usart_read(usart_dma_t* usart_dma,uint8_t *data,uint16_t count) {
	uint16_t read_count = count + 1;
	while(--read_count) {
		data[count-read_count] = usart_dma->buffer[usart_dma->read];
		usart_dma->read++;
	}
}

uint8_t usart_bytesAvailable(usart_dma_t* usart_dma) {
	return usart_dma->count;
}

void usart_dma_read_handler(usart_dma_t* usart_dma) {
	usart_dma->write = 255 - (*usart_dma->NDTR - 1);
	usart_dma->count = usart_dma->write - usart_dma->read;
	if(usart_dma->count) {
		usart_readyRead(usart_dma);
	}
}

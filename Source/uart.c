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

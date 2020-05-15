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

extern void USART_BaudRate_Set(USART_TypeDef* usart,uint32_t fpclk, uint32_t baud);


#endif /* INCLIDE_UART_H_ */

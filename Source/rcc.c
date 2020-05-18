/*
 * rcc.c
 *
 *  Created on: 13 мая 2020 г.
 *      Author: ruslan
 */

#include "rcc.h"

void rcc_init() {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; //GPIOA enable
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN; //GPIOD enable

	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN; //USART2 enable
}

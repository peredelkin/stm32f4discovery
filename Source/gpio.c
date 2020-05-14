/*
 * gpio.c
 *
 *  Created on: 13 мая 2020 г.
 *      Author: ruslan
 */

#include "gpio.h"

void gpio_afr_set(GPIO_TypeDef * gpio,uint8_t gpio_pin_n,uint8_t gpio_pin_af) {
    gpio->AFR[gpio_pin_n / 8] &= ~(uint32_t) ((0b1111 << ((gpio_pin_n % 8)*4))); //Clear AF
    gpio->AFR[gpio_pin_n / 8] |= (uint32_t) (((0b1111 & gpio_pin_af) << ((gpio_pin_n % 8)*4))); //Set AF
}

void gpio_led_init() {
	GPIOD->MODER |= GPIO_MODER_MODER12_0; //General purpose output mode
	GPIOD->MODER |= GPIO_MODER_MODER13_0; //General purpose output mode
	GPIOD->MODER |= GPIO_MODER_MODER14_0; //General purpose output mode
	GPIOD->MODER |= GPIO_MODER_MODER15_0; //General purpose output mode
}

void gpio_uart2_init() {
    //TX PA2 AF7
    GPIOA->MODER &= ~GPIO_MODER_MODER2; //Reset Mode 2
    GPIOA->MODER |= GPIO_MODER_MODER2_1; //Set Alternate Mode

    GPIOA->OTYPER &= ~GPIO_OTYPER_ODR_2; //Reset Output Type

    GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR2;// Reset Speed 2
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR2; //Set Very High Speed 2

    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR2; // Reset PUPD

    gpio_afr_set(GPIOA,2,7);

    //RX PA3 AF7
    GPIOA->MODER &= ~GPIO_MODER_MODER3; //Reset Mode 2
    GPIOA->MODER |= GPIO_MODER_MODER3_1; //Set Alternate Mode

    GPIOA->OTYPER &= ~GPIO_OTYPER_ODR_3; //Reset Output Type

    GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR3;// Reset Speed 2
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR3; //Set Very High Speed 2

    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR3; // Reset PUPD

    gpio_afr_set(GPIOA,3,7);
}


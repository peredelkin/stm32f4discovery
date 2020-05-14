/*
 * main.c
 *
 *  Created on: 13 мая 2020 г.
 *      Author: ruslan
 */
#include "main.h"

void delay_1s(void) {
	uint32_t i = 16800000;
	while (--i);
}

usart_t uart2;

void vLedTask (void *pvParameters)
{
	configASSERT( ( uint32_t ) pvParameters == 1UL );
    while(1) {
    	delay_1s();
    	GPIOD->ODR ^= GPIO_ODR_ODR_15;
    	usart_bus_write_poll(&uart2,"Hello World", 11);
    }
}

void uart2_init() {
	uart2.tx_busy = false;
	uart2.usart_bus_port = USART2;
	usart_bus_init(&uart2,SystemCoreClock/4, 9600, true, false);
}

int main() {
	delay_1s();
	rcc_init();
	gpio_led_init();
	gpio_uart2_init();
	uart2_init();
	xTaskCreateStatic(vLedTask,"vLedTask",STACK_SIZE,(void *) 1,tskIDLE_PRIORITY,xStack,&xTaskBuffer);
	vTaskStartScheduler();
}

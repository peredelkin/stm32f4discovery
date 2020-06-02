/*
 * main.c
 *
 *  Created on: 13 мая 2020 г.
 *      Author: ruslan
 */
#include "main.h"
#include "ign_angle_mg_by_cycle.h"

volatile void *ecu_addr_0[] = {
		&GPIOD->ODR,
		ign_angle_mg_by_cycle
};

void usart2_readyRead(usart_dma_t* usart_dma) {
	ecu_protocol_handler(&ecu_protocol,usart_bytesAvailable(usart_dma),ecu_addr_0);
}

void delay_1s(void) {
	uint32_t i = 16800000;
	while (--i);
}

void vUsart2_RW (void *pvParameters) {
	configASSERT( ( uint32_t ) pvParameters == 1UL );
    while(1) {
    	usart_dma_read_handler(&usart2_dma);
    	usart_dma_write_handler(&usart2_dma);
    }
}

void usart2_dma_struct_init() {
	usart2_dma.read.count = 0;
	usart2_dma.read.read_point = 0;
	usart2_dma.read.write_point = 0;
	usart2_dma.read.data = usart2_dma_rx_buffer;
	usart2_dma.read.stream = DMA1_Stream5;
	usart2_dma.read.TCIF = DMA_HISR_TCIF5;
	usart2_dma.read.HIGH = true;

	usart2_dma.write.count = 0;
	usart2_dma.write.read_point = 0;
	usart2_dma.write.write_point = 0;
	usart2_dma.write.data = usart2_dma_tx_buffer;
	usart2_dma.write.stream = DMA1_Stream6;
	usart2_dma.write.TCIF = DMA_HISR_TCIF6;
	usart2_dma.write.HIGH = true;

	usart2_dma.usart = USART2;
	usart2_dma.dma = DMA1;
	usart2_dma.usart_readyRead = (void*)(&usart2_readyRead);
}

void ecu_protocol_usart_read(void* serial,uint8_t* data,uint8_t count) {
	usart_read(serial,data,count);
}

void ecu_protocol_usart_write(void* serial,uint8_t* data,uint8_t count) {
	usart_write(serial,data,count);
}

void ecu_struct_protocol_init() {
	ecu_protocol.port = &usart2_dma;
	ecu_protocol.serial_read = &ecu_protocol_usart_read;
	ecu_protocol.serial_write = &ecu_protocol_usart_write;
}

int main() {
	delay_1s();
	rcc_init();
	gpio_led_init();
	gpio_uart2_init();
	uart2_dma_init();
	usart2_dma_struct_init();
	ecu_struct_protocol_init();
	xTaskCreateStatic(vUsart2_RW,"vUsart2_RW",VUSART2_STACK_SIZE,(void *) 1,tskIDLE_PRIORITY,vUsart2_Stack,&vUsart2_TaskBuffer);
	vTaskStartScheduler();
}

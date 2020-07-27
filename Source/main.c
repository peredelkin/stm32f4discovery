/*
 * main.c
 *
 *  Created on: 13 мая 2020 г.
 *      Author: ruslan
 */
#include "main.h"
#include "ign_angle_mg_by_cycle.h"

volatile void *ecu_addr_ptrs[] = {
		&GPIOD->ODR,
		ign_angle_mg_by_cycle
};

void usart2_readyRead(usart_dma_t* usart_dma) {
	simple_protocol_handler(&ecu_slave_protocol,usart_bytesAvailable(usart_dma));
}

void delay_1s(void) {
	uint32_t i = 16800000;
	while (--i);
}

void delay_01s(void) {
	uint32_t i = 1680000;
	while (--i);
}

void vUsart2_R (void *pvParameters) {
	configASSERT( ( uint32_t ) pvParameters == 1UL );
    while(1) {
    	usart_dma_read_handler(&usart2_dma);
    }
}

void vUsart2_W (void *pvParameters) {
	configASSERT( ( uint32_t ) pvParameters == 1UL );
    while(1) {
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

void ecu_protocol_usart_read(void* serial,uint8_t* data,uint16_t count) {
	usart_read(serial,data,(uint8_t)count);
}

void ecu_protocol_usart_write(void* serial,uint8_t* data,uint16_t count) {
	usart_write(serial,data,(uint8_t)count);
}

void ecu_protocol_crc_error(void *user_pointer, void *protocol) {

}

void ecu_protocol_data_write(void *user_pointer, void *protocol) {

}

void ecu_struct_protocol_init() {
	ecu_slave_protocol.service.addr = 1;
	ecu_slave_protocol.addr_ptrs = ecu_addr_ptrs;
	ecu_slave_protocol.read.device.port = &usart2_dma;
	ecu_slave_protocol.write.device.port = &usart2_dma;
	ecu_slave_protocol.read.device.transfer = &ecu_protocol_usart_read;
	ecu_slave_protocol.write.device.transfer = &ecu_protocol_usart_write;
	ecu_slave_protocol.crc_err.callback = &ecu_protocol_crc_error;
	ecu_slave_protocol.data_write.callback = &ecu_protocol_data_write;
}

int main() {
	delay_1s();
	rcc_init();
	gpio_led_init();
	gpio_uart2_init();
	uart2_dma_init();
	usart2_dma_struct_init();
	ecu_struct_protocol_init();
	xTaskCreateStatic(vUsart2_R,"vUsart2_R",VUSART2_STACK_SIZE,(void *) 1,tskIDLE_PRIORITY,vUsart2TX_Stack,&vUsart2TX_TaskBuffer);
	xTaskCreateStatic(vUsart2_W,"vUsart2_W",VUSART2_STACK_SIZE,(void *) 1,tskIDLE_PRIORITY,vUsart2RX_Stack,&vUsart2RX_TaskBuffer);
	vTaskStartScheduler();
}

/*
 * main.c
 *
 *  Created on: 13 мая 2020 г.
 *      Author: ruslan
 */
#include "main.h"

uint16_t ecu_read_count;
uint16_t ecu_read_count_end;
ecu_frame_t ecu_read;

#define USART2_DMA_RX_BUFFER_SIZE 256
uint8_t usart2_dma_rx_read_point = 0;
uint8_t usart2_dma_rx_write_point = 0;
uint8_t usart2_dma_rx_read_count = 0;
uint8_t usart2_dma_rx_buffer[USART2_DMA_RX_BUFFER_SIZE];

void usart2_read(uint8_t *data,uint16_t count) {
	uint16_t read_count = count + 1;
	while(--read_count) {
		data[count-read_count] = usart2_dma_rx_buffer[usart2_dma_rx_read_point];
		usart2_dma_rx_read_point++;
	}
}

void usart2_readyRead() {
    if(ecu_read_count_end != ecu_read_count) {
        if(usart2_dma_rx_read_count >= (ecu_read_count_end - ecu_read_count)) {
        	usart2_read(&(((uint8_t*)(&ecu_read))[ecu_read_count]),(ecu_read_count_end - ecu_read_count));
            ecu_read_count = ecu_read_count_end;
            if(ecu_read_count == (ECU_CMD_ADDR_COUNT + ECU_SERVICE_DATA_COUNT)) {
                ecu_read_count_end += ecu_read.service_data.count + ECU_CRC_COUNT;
            }
            if(ecu_read_count == ((ECU_CMD_ADDR_COUNT + ECU_SERVICE_DATA_COUNT + ECU_CRC_COUNT) + ecu_read.service_data.count)) {
                uint16_t crc_calc = crc16_ccitt((uint8_t*)(&ecu_read),ecu_read_count_end - ECU_CRC_COUNT);
                uint16_t crc_read = *(uint16_t*)(&ecu_read.data[ecu_read.service_data.count]);
                if(crc_calc == crc_read) {
                    GPIOD->ODR ^= GPIO_ODR_ODR_15;
                } else {
                	GPIOD->ODR ^= GPIO_ODR_ODR_14;
                }
            }
        }
    } else {
        ecu_read_count = 0;
        ecu_read_count_end = ECU_CMD_ADDR_COUNT + ECU_SERVICE_DATA_COUNT;
    }
}

void usart2_dma_read() {
	usart2_dma_rx_write_point = 255 - (DMA1_Stream5->NDTR - 1);
	usart2_dma_rx_read_count = usart2_dma_rx_write_point - usart2_dma_rx_read_point;
	if(usart2_dma_rx_read_count) {
		usart2_readyRead();
	}
}

void delay_1s(void) {
	uint32_t i = 16800000;
	while (--i);
}

void vLedTask (void *pvParameters)
{
	configASSERT( ( uint32_t ) pvParameters == 1UL );
    while(1) {
    	usart2_dma_read();
    }
}

void uart2_init() {
	USART2->CR1 = 	USART_CR1_OVER8 |	/* Oversampling by 8		*/
					USART_CR1_TE |		/* Transmitter enable		*/
					USART_CR1_RE;		/* Receiver enable			*/

	USART2->CR3 =	USART_CR3_DMAT |	/* DMA enable transmitter	*/
					USART_CR3_DMAR;		/* DMA enable receiver		*/

	USART_BaudRate_Set(USART2,SystemCoreClock/4,9600);

	//USART2_RX Channel 4 Stream 5
	//USART2_TX Channel 4 Stream 6

	DMA1_Stream5->NDTR = USART2_DMA_RX_BUFFER_SIZE;
	DMA1_Stream5->PAR = (uint32_t)(&USART2->DR);
	DMA1_Stream5->MAR[0] = (uint32_t)usart2_dma_rx_buffer;

	DMA1_Stream5->CR =	DMA_SxCR_CHSEL_2 |	/* Channel 4 		*/
						DMA_SxCR_MINC |		/* Memory increment	*/
						DMA_SxCR_CIRC |		/* Circular mode	*/
						DMA_SxCR_EN;		/* Stream enable	*/

	USART2->CR1 |=	USART_CR1_UE;			/* USART2 enable	*/
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

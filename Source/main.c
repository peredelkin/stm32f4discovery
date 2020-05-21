/*
 * main.c
 *
 *  Created on: 13 мая 2020 г.
 *      Author: ruslan
 */
#include "main.h"

ecu_rw_t ecu_read;

uint8_t usart2_dma_rx_buffer[DMA_RX_BUFFER_SIZE];

void usart2_readyRead(usart_dma_t* usart_dma);

usart_dma_t usart2_dma;

void usart2_dma_struct_init() {
	usart2_dma.read.count = 0;
	usart2_dma.read.read_point = 0;
	usart2_dma.read.write_point = 0;
	usart2_dma.read.stream = DMA1_Stream5;
	usart2_dma.usart_readyRead = (void*)(&usart2_readyRead);
}

void ecu_read_handler(ecu_rw_t* ecu_rw,usart_dma_t* usart_dma) {
    if(ecu_rw->count_end != ecu_rw->count) {
        if(usart_bytesAvailable(usart_dma) >= (ecu_rw->count_end - ecu_rw->count)) {
        	usart_read(usart_dma,&((uint8_t*)(&ecu_rw->frame))[ecu_rw->count],(ecu_rw->count_end - ecu_rw->count));
        	ecu_rw->count = ecu_rw->count_end;
            if(ecu_rw->count == (ECU_CMD_ADDR_COUNT + ECU_SERVICE_DATA_COUNT)) {
            	ecu_rw->count_end += ecu_rw->frame.service_data.count + ECU_CRC_COUNT;
            }
            if(ecu_rw->count == ((ECU_CMD_ADDR_COUNT + ECU_SERVICE_DATA_COUNT + ECU_CRC_COUNT) + ecu_rw->frame.service_data.count)) {
                uint16_t crc_calc = crc16_ccitt((uint8_t*)(&ecu_rw->frame),ecu_rw->count_end - ECU_CRC_COUNT);
                uint16_t crc_read = *(uint16_t*)(&ecu_rw->frame.data[ecu_rw->frame.service_data.count]);
                if(crc_calc == crc_read) {
                    GPIOD->ODR ^= GPIO_ODR_ODR_15;
                } else {
                	GPIOD->ODR ^= GPIO_ODR_ODR_14;
                }
            }
        }
    } else {
    	ecu_rw->count = 0;
    	ecu_rw->count_end = ECU_CMD_ADDR_COUNT + ECU_SERVICE_DATA_COUNT;
    }
}

void usart2_readyRead(usart_dma_t* usart_dma) {
	ecu_read_handler(&ecu_read,usart_dma);
}

void delay_1s(void) {
	uint32_t i = 16800000;
	while (--i);
}

void vUsart2_Read (void *pvParameters)
{
	configASSERT( ( uint32_t ) pvParameters == 1UL );
    while(1) {
    	usart_dma_read_handler(&usart2_dma);
    }
}

void uart2_dma_init() {
	USART2->CR1 = 	USART_CR1_OVER8 |	/* Oversampling by 8		*/
					USART_CR1_TE |		/* Transmitter enable		*/
					USART_CR1_RE;		/* Receiver enable			*/

	USART2->CR3 =	USART_CR3_DMAT |	/* DMA enable transmitter	*/
					USART_CR3_DMAR;		/* DMA enable receiver		*/

	USART_BaudRate_Set(USART2,SystemCoreClock/4,9600);

	//USART2_RX Channel 4 Stream 5
	DMA1_Stream5->NDTR = DMA_RX_BUFFER_SIZE;
	DMA1_Stream5->PAR = (uint32_t)(&USART2->DR);
	DMA1_Stream5->MAR[0] = (uint32_t)usart2_dma_rx_buffer;

	//DMA1_Stream5->FCR = DMA_SxFCR_DMDIS;	/* Direct mode disabled	*/

	DMA1_Stream5->CR =	DMA_SxCR_CHSEL_2 |	/* Channel 4			*/
						DMA_SxCR_PL |		/* Very high Priority	*/
						DMA_SxCR_MINC |		/* Memory increment		*/
						DMA_SxCR_CIRC |		/* Circular mode		*/
						DMA_SxCR_EN;		/* Stream enable		*/

	//USART2_TX Channel 4 Stream 6

	USART2->CR1 |=	USART_CR1_UE;			/* USART2 enable		*/
}

int main() {
	delay_1s();
	rcc_init();
	gpio_led_init();
	gpio_uart2_init();
	uart2_dma_init();
	usart2_dma_struct_init();
	xTaskCreateStatic(vUsart2_Read,"vUsart2_Read",STACK_SIZE,(void *) 1,tskIDLE_PRIORITY,xStack,&xTaskBuffer);
	vTaskStartScheduler();
}

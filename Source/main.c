/*
 * main.c
 *
 *  Created on: 13 мая 2020 г.
 *      Author: ruslan
 */
#include "main.h"

ecu_rw_t ecu_read;

#pragma pack(1)
uint8_t usart2_dma_rx_buffer[DMA_RX_BUFFER_SIZE];
uint8_t usart2_dma_tx_buffer[DMA_TX_BUFFER_SIZE];
#pragma pack()

usart_dma_t usart2_dma;

volatile void *ecu_addr_0[] = {
		&GPIOC->ODR,
		&GPIOD->ODR
};

void ecu_read_frame(ecu_rw_t *ecu_r,volatile void **data) {
	uint8_t type = ecu_r->frame.cmd_addr.cmd & 0x0F;
	uint16_t start = (ecu_r->frame.service_data.start / type);
	uint16_t count = (ecu_r->frame.service_data.count / type) + 1;
	uint16_t read_point = 0;
	uint16_t write_point = 0;
	while (--count) {
		switch (type) {
		case 1: ((uint8_t*)(data[ecu_r->frame.cmd_addr.addr]))[start + write_point] = *(uint8_t*)(&ecu_r->frame.data[read_point]);
			break;
		case 2: ((uint16_t*)(data[ecu_r->frame.cmd_addr.addr]))[start + write_point] = *(uint16_t*)(&ecu_r->frame.data[read_point]);
			break;
		case 4: ((uint32_t*)(data[ecu_r->frame.cmd_addr.addr]))[start + write_point] = *(uint32_t*)(&ecu_r->frame.data[read_point]);
			break;
		default:
			return;
		};
		read_point += type;
		write_point++;
	}
}

void ecu_read_handler(ecu_rw_t* ecu_r,usart_dma_t* usart_dma) {
    if(ecu_r->count_end != ecu_r->count) {
        if(usart_bytesAvailable(usart_dma) >= (ecu_r->count_end - ecu_r->count)) {
        	usart_read(usart_dma,&((uint8_t*)(&ecu_r->frame))[ecu_r->count],(ecu_r->count_end - ecu_r->count));
        	ecu_r->count = ecu_r->count_end;
            if(ecu_r->count == (ECU_CMD_ADDR_COUNT + ECU_SERVICE_DATA_COUNT)) {
            	ecu_r->count_end += ecu_r->frame.service_data.count + ECU_CRC_COUNT;
            }
            if(ecu_r->count == ((ECU_CMD_ADDR_COUNT + ECU_SERVICE_DATA_COUNT + ECU_CRC_COUNT) + ecu_r->frame.service_data.count)) {
                uint16_t crc_calc = crc16_ccitt((uint8_t*)(&ecu_r->frame),ecu_r->count_end - ECU_CRC_COUNT);
                uint16_t crc_read = *(uint16_t*)(&ecu_r->frame.data[ecu_r->frame.service_data.count]);
                if(crc_calc == crc_read) {
                	if(ecu_r->frame.cmd_addr.cmd & 0x10) {
                		usart_write(usart_dma,(uint8_t*)(&ecu_r->frame),ecu_r->count);
                		ecu_read_frame(ecu_r,ecu_addr_0);
                	}
                } else {
                	//crc incorrect
                }
            }
        }
    } else {
    	ecu_r->count = 0;
    	ecu_r->count_end = ECU_CMD_ADDR_COUNT + ECU_SERVICE_DATA_COUNT;
    }
}

void usart2_readyRead(usart_dma_t* usart_dma) {
	ecu_read_handler(&ecu_read,usart_dma);
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
	DMA1_Stream5->CR =	DMA_SxCR_CHSEL_2 |	/* Channel 4			*/
						DMA_SxCR_PL |		/* Very high Priority	*/
						DMA_SxCR_MINC |		/* Memory increment		*/
						DMA_SxCR_CIRC |		/* Circular mode		*/
						DMA_SxCR_EN;		/* Stream enable		*/

	//USART2_TX Channel 4 Stream 6
	DMA1_Stream6->PAR = (uint32_t)(&USART2->DR);
	DMA1_Stream6->CR =	DMA_SxCR_CHSEL_2 |	/* Channel 4			*/
						DMA_SxCR_PL |		/* Very high Priority	*/
						DMA_SxCR_MINC |		/* Memory increment		*/
						DMA_SxCR_DIR_0;		/* Memory-to-peripheral	*/


	USART2->CR1 |=	USART_CR1_UE;			/* USART2 enable		*/
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

int main() {
	delay_1s();
	rcc_init();
	gpio_led_init();
	gpio_uart2_init();
	uart2_dma_init();
	usart2_dma_struct_init();
	xTaskCreateStatic(vUsart2_RW,"vUsart2_RW",VUSART2_STACK_SIZE,(void *) 1,tskIDLE_PRIORITY,vUsart2_Stack,&vUsart2_TaskBuffer);
	vTaskStartScheduler();
}

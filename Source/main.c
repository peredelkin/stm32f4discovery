/*
 * main.c
 *
 *  Created on: 13 мая 2020 г.
 *      Author: ruslan
 */
#include "main.h"

volatile void *ecu_addr_0[] = {
		&GPIOC->ODR,
		&GPIOD->ODR
};

void ecu_read_frame_data(ecu_rw_t *ecu_r,volatile void **data) {
	uint8_t type = ecu_r->frame.cmd_addr.cmd & ECU_DATA_TYPE_MASK;
	uint16_t write_start = (ecu_r->frame.service_data.start / type);
	uint16_t write_count = (ecu_r->frame.service_data.count / type) + 1;
	uint16_t read_point = 0;
	uint16_t write_point = 0;
	while (--write_count) {
		switch (type) {
		case ECU_DATA_TYPE_8: ((uint8_t*)(data[ecu_r->frame.cmd_addr.addr]))[write_start + write_point] = *(uint8_t*)(&ecu_r->frame.data[read_point]);
			break;
		case ECU_DATA_TYPE_16: ((uint16_t*)(data[ecu_r->frame.cmd_addr.addr]))[write_start + write_point] = *(uint16_t*)(&ecu_r->frame.data[read_point]);
			break;
		case ECU_DATA_TYPE_32: ((uint32_t*)(data[ecu_r->frame.cmd_addr.addr]))[write_start + write_point] = *(uint32_t*)(&ecu_r->frame.data[read_point]);
			break;
		default:
			return;
		};
		read_point += type;
		write_point++;
	}
}

void ecu_write_frame_data(ecu_rw_t* ecu_w,volatile void **data,ecu_rw_t* ecu_r) {
	ecu_w->frame.cmd_addr.cmd = ecu_r->frame.cmd_addr.cmd;
	ecu_w->frame.cmd_addr.addr = ecu_r->frame.cmd_addr.addr;
	ecu_w->frame.service_data.start = ecu_r->frame.service_data.start;
	ecu_w->frame.service_data.count = ecu_r->frame.service_data.count;
	uint8_t type = ecu_w->frame.cmd_addr.cmd & ECU_DATA_TYPE_MASK;
	uint16_t read_start = (ecu_w->frame.service_data.start / type);
	uint16_t read_count = (ecu_w->frame.service_data.count / type) + 1;
	uint16_t write_point = 0;
	uint16_t read_point = 0;
	while (--read_count) {
		switch (type) {
		case ECU_DATA_TYPE_8: *(uint8_t*)(&ecu_w->frame.data[write_point]) = ((uint8_t*)(data[ecu_w->frame.cmd_addr.addr]))[read_start + read_point];
			break;
		case ECU_DATA_TYPE_16: *(uint16_t*)(&ecu_w->frame.data[write_point]) = ((uint16_t*)(data[ecu_w->frame.cmd_addr.addr]))[read_start + read_point];
			break;
		case ECU_DATA_TYPE_32: *(uint32_t*)(&ecu_w->frame.data[write_point]) = ((uint32_t*)(data[ecu_w->frame.cmd_addr.addr]))[read_start + read_point];
			break;
		default:
			return;
		};
		write_point += type;
		read_point++;
	}
	ecu_w->count = ECU_CMD_ADDR_COUNT + ECU_SERVICE_DATA_COUNT + ecu_w->frame.service_data.count;
	*(uint16_t*)(&ecu_w->frame.data[ecu_w->frame.service_data.count]) = crc16_ccitt((uint8_t*)(&ecu_w->frame),ecu_w->count);
	ecu_w->count += ECU_CRC_COUNT;
}

void ecu_protocol_handler(ecu_protocol_t* protocol,usart_dma_t* usart_dma) {
    if(protocol->read.count_end != protocol->read.count) {
        if(usart_bytesAvailable(usart_dma) >= (protocol->read.count_end - protocol->read.count)) {
        	usart_read(usart_dma,&((uint8_t*)(&protocol->read.frame))[protocol->read.count],(protocol->read.count_end - protocol->read.count));
        	protocol->read.count = protocol->read.count_end;
			switch (protocol->cmd_type) {
			case ECU_CMD_TYPE_DEF: {
				protocol->cmd_type = (uint8_t) ((protocol->read.frame.cmd_addr.cmd & ECU_CMD_MASK));
				switch (protocol->cmd_type) {
				case ECU_CMD_WRITE:
					protocol->read.count_end += protocol->read.frame.service_data.count + ECU_CRC_COUNT;
					break;
				case ECU_CMD_READ:
					protocol->read.count_end += ECU_CRC_COUNT;
					break;
				default:
					break;
				};
			}
				break;
			case ECU_CMD_WRITE: {
				protocol->crc_read = *(uint16_t*)(&protocol->read.frame.data[protocol->read.frame.service_data.count]);
				protocol->crc_calc = crc16_ccitt((uint8_t*)(&protocol->read.frame),protocol->read.count_end - ECU_CRC_COUNT);
				if(protocol->crc_read == protocol->crc_calc) {
					ecu_read_frame_data(&protocol->read,ecu_addr_0);
				}
			}
				break;
			case ECU_CMD_READ: {
				protocol->crc_read = *(uint16_t*)(&protocol->read.frame.data[0]);
				protocol->crc_calc = crc16_ccitt((uint8_t*)(&protocol->read.frame),protocol->read.count_end - ECU_CRC_COUNT);
				if(protocol->crc_read == protocol->crc_calc) {
					ecu_write_frame_data(&protocol->write,ecu_addr_0,&protocol->read);
					usart_write(usart_dma,(uint8_t*)(&protocol->write.frame),protocol->write.count);
				}
			}
				break;
			default:
				break;
			};
        }
    } else {
    	protocol->cmd_type = ECU_CMD_TYPE_DEF;
    	protocol->read.count = 0;
    	protocol->read.count_end = ECU_CMD_ADDR_COUNT + ECU_SERVICE_DATA_COUNT;
    }
}

void usart2_readyRead(usart_dma_t* usart_dma) {
	ecu_protocol_handler(&ecu_protocol,usart_dma);
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

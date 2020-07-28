/*
 * main.h
 *
 *  Created on: 13 мая 2020 г.
 *      Author: ruslan
 */

#ifndef INCLIDE_MAIN_H_
#define INCLIDE_MAIN_H_

#include "stm32f4xx.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

#include "rcc.h"
#include "gpio.h"
#include "uart.h"

#include "simple_protocol.h"

#define VUSART2_STACK_SIZE (configMINIMAL_STACK_SIZE * 2)

StaticTask_t vUsart2RX_TaskBuffer;
StackType_t vUsart2RX_Stack[ VUSART2_STACK_SIZE ];

//StaticTask_t vUsart2TX_TaskBuffer;
//StackType_t vUsart2TX_Stack[ VUSART2_STACK_SIZE ];

simple_protocol_link_layer_t ecu_slave_protocol;

uint8_t usart2_dma_rx_buffer[DMA_RX_BUFFER_SIZE];
uint8_t usart2_dma_tx_buffer[DMA_TX_BUFFER_SIZE];

usart_dma_t usart2_dma;


//==============================================================================
#if configSUPPORT_STATIC_ALLOCATION
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configTIMER_TASK_STACK_DEPTH];

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize) {
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}
#endif

#if configSUPPORT_STATIC_ALLOCATION && configUSE_TIMERS
static StaticTask_t xTimerTaskTCBBuffer;
static StackType_t xTimerStack[configTIMER_TASK_STACK_DEPTH];

void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize) {
  *ppxTimerTaskTCBBuffer = &xTimerTaskTCBBuffer;
  *ppxTimerTaskStackBuffer = &xTimerStack[0];
  *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}
#endif

#if(  configCHECK_FOR_STACK_OVERFLOW > 0 )
void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) xTask;
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
#endif
//==============================================================================


void uart2_dma_init() {
	USART2->CR1 = 	USART_CR1_OVER8 |	/* Oversampling by 8		*/
					USART_CR1_TE |		/* Transmitter enable		*/
					USART_CR1_RE;		/* Receiver enable			*/

	USART2->CR3 =	USART_CR3_DMAT |	/* DMA enable transmitter	*/
					USART_CR3_DMAR;		/* DMA enable receiver		*/

	USART_BaudRate_Set(USART2,SystemCoreClock/4,115200);

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

#endif /* INCLIDE_MAIN_H_ */

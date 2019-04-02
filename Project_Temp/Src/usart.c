/*
 * usart.c
 *
 *  Created on: Jan 20, 2019
 *      Author: QUAN
 */

#include "usart.h"

UART_HandleTypeDef 			huart3;
UART_HandleTypeDef 			huart6;
GPIO_InitTypeDef GPIO_InitStruct = {0};

void MX_USARTx_Init(USART_TypeDef * USARTx, uint32_t baudrate)
{
	if (USARTx == USART3) {

		huart3.Instance 				= USART3;
		huart3.Init.BaudRate 			= baudrate;
		huart3.Init.WordLength 			= UART_WORDLENGTH_8B;
		huart3.Init.StopBits 			= UART_STOPBITS_1;
		huart3.Init.Parity 				= UART_PARITY_NONE;
		huart3.Init.Mode 				= UART_MODE_TX_RX;
		huart3.Init.HwFlowCtl 			= UART_HWCONTROL_NONE;
		huart3.Init.OverSampling 		= UART_OVERSAMPLING_16;

		if (HAL_UART_Init(&huart3) != HAL_OK) {
			Error_Handler();
		}
	} else if (USARTx == USART6) {

		huart6.Instance 				= USART6;
		huart6.Init.BaudRate 			= baudrate;
		huart6.Init.WordLength 			= UART_WORDLENGTH_8B;
		huart6.Init.StopBits 			= UART_STOPBITS_1;
		huart6.Init.Parity				= UART_PARITY_NONE;
		huart6.Init.Mode 				= UART_MODE_TX_RX;
		huart6.Init.HwFlowCtl 			= UART_HWCONTROL_NONE;
		huart6.Init.OverSampling 		= UART_OVERSAMPLING_16;

		if (HAL_UART_Init(&huart6) != HAL_OK) {
			Error_Handler();
		}
	}
	uint8_t buff[]="--------- usart init success ---------\r\n";
	uint16_t length = sizeof(buff)/sizeof(uint8_t) - 1;
	HAL_UART_Transmit(&huart3, buff, length, 100);
}


/**
* @brief UART MSP Initialization
* This function configures the hardware resources used in this example
* @param huart: UART handle pointer
* @retval None
*/
void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
	if(huart->Instance==USART3) {
		__HAL_RCC_GPIOB_CLK_ENABLE();
		__HAL_RCC_USART3_CLK_ENABLE();
		/**USART3 GPIO Configuration
		PB10     -----. USART3_TX
		PB11     -----. USART3_RX
		*/
		GPIO_InitStruct.Pin       = USART3_TX_Pin|USART3_RX_Pin;
		GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull      = GPIO_PULLUP;
		GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
		HAL_GPIO_Init(USART3_Port, &GPIO_InitStruct);

	} else if(huart->Instance==USART6) {
		__HAL_RCC_GPIOG_CLK_ENABLE();
		__HAL_RCC_USART6_CLK_ENABLE();
		/**USART6 GPIO Configuration
		PG14     -----. USART6_TX
		PG9     -----. USART6_RX
		*/
		GPIO_InitStruct.Pin       = USART6_TX_Pin|USART6_RX_Pin;
		GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull      = GPIO_PULLUP;
		GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF8_USART6;
		HAL_GPIO_Init(USART6_Port, &GPIO_InitStruct);

	}
}

/**
* @brief UART MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param huart: UART handle pointer
* @retval None
*/

void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{

	if(huart->Instance==USART3) {
		__HAL_RCC_USART3_CLK_DISABLE();

		/**USART3 GPIO Configuration
		PB10     -----. USART3_TX
		PB11     -----. USART3_RX
		*/
		HAL_GPIO_DeInit(USART3_Port, USART3_TX_Pin|USART3_RX_Pin);

	} else if(huart->Instance==USART6) {
		/* USER CODE BEGIN USART6_MspDeInit 0 */

		/* USER CODE END USART6_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_USART6_CLK_DISABLE();

		/**USART6 GPIO Configuration
		PG14     -----. USART6_TX
		PG9     -----. USART6_RX
		*/
		HAL_GPIO_DeInit(USART6_Port, USART6_TX_Pin|USART6_RX_Pin);
	}
}

int uasrtx_send_string(UART_HandleTypeDef * huart, uint8_t *str, uint16_t len)
{
	if(osOK != osThreadSuspendAll()) {
		osThreadYield();
		return -1;
	}
	if(huart->Instance == USART3) {
		HAL_UART_Transmit(&huart3, str, len, 500);
	} else if(huart->Instance == USART6) {
		HAL_UART_Transmit(&huart6, str, len, 500);
	}
	/* The operation is complete.  Restart the RTOS kernel.  We want to force a context
	 * switch - but there is no point if resuming the scheduler
	 * caused a context switch already. */
	if(osOK != osThreadResumeAll()) {
		osThreadYield();		// force switch
		return -1;
	}
	return 0;
}

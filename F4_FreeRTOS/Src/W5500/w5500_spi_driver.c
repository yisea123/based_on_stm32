/*
 * w5500_driver.c
 *
 *  Created on: Mar 21, 2019
 *      Author: shush
 */
#include "w5500_driver.h"



SPI_HandleTypeDef hspi1;
DMA_HandleTypeDef hdma_spi1_tx;
DMA_HandleTypeDef hdma_spi1_rx;


static void MX_SPI1_Init(void)
{
	hspi1.Instance = SPI1;
	hspi1.Init.Mode = SPI_MODE_MASTER;
	hspi1.Init.Direction = SPI_DIRECTION_2LINES;
	hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi1.Init.NSS = SPI_NSS_HARD_OUTPUT;
	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
	hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi1.Init.CRCPolynomial = 10;
	if (HAL_SPI_Init(&hspi1) != HAL_OK)
	{
		Error_Handler();
	}
}
/**
* @brief SPI MSP Initialization
* This function configures the hardware resources used in this example
* @param hspi: SPI handle pointer
* @retval None
*/
void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi)
{

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	if(hspi->Instance==SPI1)
	{
		SPIx_CLK_ENABLE();

		SPIx_SCK_GPIO_CLK_ENABLE();
		/**SPI1 GPIO Configuration
		PB5     ------> SPI1_MOSI
		PB4     ------> SPI1_MISO
		PA15     ------> SPI1_NSS
		PA5     ------> SPI1_SCK
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_4;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_15|GPIO_PIN_5;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		/* SPI1 DMA Init */
		/* SPI1_TX Init */
		hdma_spi1_tx.Instance 					= DMA2_Stream3;
		hdma_spi1_tx.Init.Channel 				= DMA_CHANNEL_3;
		hdma_spi1_tx.Init.Direction 			= DMA_MEMORY_TO_PERIPH;
		hdma_spi1_tx.Init.PeriphInc 			= DMA_PINC_DISABLE;
		hdma_spi1_tx.Init.MemInc 				= DMA_MINC_ENABLE;
		hdma_spi1_tx.Init.PeriphDataAlignment 	= DMA_PDATAALIGN_BYTE;
		hdma_spi1_tx.Init.MemDataAlignment 		= DMA_MDATAALIGN_BYTE;
		hdma_spi1_tx.Init.Mode 					= DMA_NORMAL;
		hdma_spi1_tx.Init.Priority 				= DMA_PRIORITY_MEDIUM;
		hdma_spi1_tx.Init.FIFOMode 				= DMA_FIFOMODE_DISABLE;
		if (HAL_DMA_Init(&hdma_spi1_tx) != HAL_OK)
		{
			Error_Handler();
		}

		__HAL_LINKDMA(hspi,hdmatx,hdma_spi1_tx);

		/* SPI1_RX Init */
		hdma_spi1_rx.Instance 					= DMA2_Stream0;
		hdma_spi1_rx.Init.Channel 				= DMA_CHANNEL_3;
		hdma_spi1_rx.Init.Direction 			= DMA_PERIPH_TO_MEMORY;
		hdma_spi1_rx.Init.PeriphInc 			= DMA_PINC_DISABLE;
		hdma_spi1_rx.Init.MemInc 				= DMA_MINC_ENABLE;
		hdma_spi1_rx.Init.PeriphDataAlignment 	= DMA_PDATAALIGN_BYTE;
		hdma_spi1_rx.Init.MemDataAlignment 		= DMA_MDATAALIGN_BYTE;
		hdma_spi1_rx.Init.Mode 					= DMA_NORMAL;
		hdma_spi1_rx.Init.Priority 				= DMA_PRIORITY_MEDIUM;
		hdma_spi1_rx.Init.FIFOMode 				= DMA_FIFOMODE_DISABLE;
		if (HAL_DMA_Init(&hdma_spi1_rx) != HAL_OK)
		{
			Error_Handler();
		}

		__HAL_LINKDMA(hspi,hdmarx,hdma_spi1_rx);

		/* SPI1 interrupt Init */
		HAL_NVIC_SetPriority(SPI1_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(SPI1_IRQn);
		/* USER CODE BEGIN SPI1_MspInit 1 */

		/* USER CODE END SPI1_MspInit 1 */
	}

}

/**
* @brief SD MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hsd: SD handle pointer
* @retval None
*/

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* hspi)
{

	if(hspi->Instance==SPI1)
	{
		__HAL_RCC_SPI1_CLK_DISABLE();

		/**SPI1 GPIO Configuration
		PB5     ------> SPI1_MOSI
		PB4     ------> SPI1_MISO
		PA15     ------> SPI1_NSS
		PA5     ------> SPI1_SCK
		*/
		HAL_GPIO_DeInit(GPIOB, GPIO_PIN_5|GPIO_PIN_4);

		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_15|GPIO_PIN_5);

		/* SPI1 DMA DeInit */
		HAL_DMA_DeInit(hspi->hdmatx);
		HAL_DMA_DeInit(hspi->hdmarx);

		/* SPI1 interrupt DeInit */
		HAL_NVIC_DisableIRQ(SPI1_IRQn);
		/* USER CODE BEGIN SPI1_MspDeInit 1 */

		/* USER CODE END SPI1_MspDeInit 1 */
	}

}


void w5500_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	MX_SPI1_Init();

	W5500_GPIO_CLK_ENABLE();

	GPIO_InitStruct.Pin   = W5500_NRST_PIN;
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(W5500_GPIO_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(W5500_GPIO_PORT, W5500_NRST_PIN, GPIO_PIN_SET);

	GPIO_InitStruct.Pin   = W5500_INIT_PIN;
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(W5500_GPIO_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(W5500_GPIO_PORT, W5500_INIT_PIN, GPIO_PIN_SET);

}


/*
 * w5500_driver.h
 *
 *  Created on: Mar 21, 2019
 *      Author: shush
 */
/******************************************************
 * spi1_nss ---> PA15
 * spi1_sclk ---> PA5
 * spi1_miso ---> PB4
 * spi1_mosi ---> PB5
 * w5500_nrst ---> PC13
 * w5500_init ---> PC1
 ******************************************************/
#ifndef W5500_DRIVER_H_
#define W5500_DRIVER_H_

#include "main.h"
#include "stm32f4xx_hal_spi.h"

/* Definition for SPIx clock resources ------------------------------------------*/
#define SPIx                             SPI1
#define SPIx_CLK_ENABLE()                __HAL_RCC_SPI1_CLK_ENABLE()
#define DMAx_CLK_ENABLE()                __HAL_RCC_DMA1_CLK_ENABLE()
#define SPIx_SCK_GPIO_CLK_ENABLE()       do {									\
												__HAL_RCC_GPIOB_CLK_ENABLE();	\
												__HAL_RCC_GPIOA_CLK_ENABLE();	\
											} while(0)

#define W5500_GPIO_CLK_ENABLE()      	__HAL_RCC_GPIOC_CLK_ENABLE()

#define SPIx_FORCE_RESET()               __HAL_RCC_SPI1_FORCE_RESET()
#define SPIx_RELEASE_RESET()             __HAL_RCC_SPI1_RELEASE_RESET()

/* Definition for W5500 Pins -------------------------------------------------------*/
#define W5500_INIT_PIN                     	GPIO_PIN_1
#define W5500_NRST_PIN                     	GPIO_PIN_13
#define W5500_GPIO_PORT               		GPIOC
#define W5500_GPIO_CLK_ENABLE()				__HAL_RCC_GPIOC_CLK_ENABLE()
/* Definition for SPIx Pins -------------------------------------------------------*/
#define SPIx_SCK_PIN                     GPIO_PIN_3
#define SPIx_SCK_GPIO_PORT               GPIOA
#define SPIx_MISO_PIN                    GPIO_PIN_4
#define SPIx_MISO_GPIO_PORT              GPIOB
#define SPIx_MOSI_PIN                    GPIO_PIN_5
#define SPIx_MOSI_GPIO_PORT              GPIOB
#define SPIx_MOSI_NSS                    GPIO_PIN_15
#define SPIx_NSS_GPIO_PORT               GPIOA
/* Definition for SPIx's DMA ------------------------------------------------------*/
#define SPIx_TX_DMA_STREAM               DMA2_Stream3
#define SPIx_RX_DMA_STREAM               DMA2_Stream0
#define SPIx_TX_DMA_CHANNEL              DMA_CHANNEL_3
#define SPIx_RX_DMA_CHANNEL              DMA_CHANNEL_3



/* Definition for SPIx's NVIC */
#define SPIx_IRQn                        SPI2_IRQn
#define SPIx_IRQHandler                  SPI2_IRQHandler

#define SPIx_DMA_TX_IRQn                 DMA1_Stream4_IRQn
#define SPIx_DMA_RX_IRQn                 DMA1_Stream3_IRQn

#define SPIx_DMA_TX_IRQHandler           DMA1_Stream4_IRQHandler
#define SPIx_DMA_RX_IRQHandler           DMA1_Stream3_IRQHandler

/* Size of buffer */
#define BUFFERSIZE                       (COUNTOF(aTxBuffer) - 1)

/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
/* Exported functions ------------------------------------------------------- */


void w5500_init(void);


#endif /* W5500_DRIVER_H_ */

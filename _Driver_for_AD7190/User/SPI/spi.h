/*****************************************************************************
*
*
*
******************************************************************************/

#ifndef __SPI_H
#define __SPI_H

#include "stm32f10x_conf.h"
#include "delay.h"

/******************************* Pin alias description ***********************/
#define SPI1_PORT                   GPIOA
#define SPI1_SCK                    GPIO_Pin_5
#define SPI1_MOSI                   GPIO_Pin_7
#define SPI1_MISO                   GPIO_Pin_6
#define SPI1_NSS                    GPIO_Pin_4

#define _CS_H()                     GPIO_SetBits(SPI1_PORT, SPI1_NSS)
#define _CS_L()                     GPIO_ResetBits(SPI1_PORT, SPI1_NSS)

/******************************* if define SPI control by sofware **********************************************/
#if SPI_HARDWARE == false
	#define _SCLK_H()				GPIO_SetBits(SPI1_PORT, SPI1_SCK)
	#define _SCLK_L()               GPIO_ResetBits(SPI1_PORT, SPI1_SCK)
	
	#define _MOSI_H()				GPIO_SetBits(SPI1_PORT, SPI1_MOSI)
	#define _MOSI_L()               GPIO_ResetBits(SPI1_PORT, SPI1_MOSI)
	
	#define _Read_MISO()			GPIO_ReadInputDataBit(SPI1_PORT, SPI1_MISO)

#endif

/*************************************** need to consider **************************
typedef struct SPI_DataAnalysis_Init
{
	uint8_t SPI1_SendBuff[20];
	uint8_t SPI1_ReadBuff[20];

}SPI_DataAnalysis;
***********************************************************************************/
void SPI1_gpioConfig(void);
void SPI1_Config(void);
void SPI1_Init(void);
static uint8_t _SPI_ReadWrite(unsigned char* data, unsigned char bytesNumber)

//void SPI1_SendByte(uint8_t byte);  
// void SPI1_SendString(SPI_DataAnalysis *String, uint8_t length);
//uint8_t SPI1_ReadByte(void);
// SPI_DataAnalysis SPI1_ReadString(void);
// uint8_t SPI2_ReadWriteByte(uint8_t TxData);

#endif

/************************************* END LINE ************************************************/




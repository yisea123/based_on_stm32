#ifndef __SPI_H
#define __SPI_H

#include "stm32f10x_conf.h"

extern void delay_us(u32 nus);			//声明外部延时函数
extern void delay_ms(u16 nms);
//-------SPI端口预定义----------------------------------
#define SPI_CLK_Delay() 		    Delayus(10)

#define	SPI_GPIO_Port_RCC			RCC_APB2Periph_GPIOA
//#define	AD7175_SYNC_Port_RCC		RCC_APB2Periph_GPIOA
//#define AD7175_SYNC_Port		    GPIOB

#define SPI_CS_GPIO_Port		    GPIOA
#define SPI_CS_GPIO_Pin			    GPIO_Pin_4

#define SPI_CLK_GPIO_Port		    GPIOA
#define SPI_CLK_GPIO_Pin		    GPIO_Pin_3

#define SPI_MISO_GPIO_Port	        GPIOA
#define SPI_MISO_GPIO_Pin		    GPIO_Pin_6//read     

#define SPI_MOSI_GPIO_Port	        GPIOA
#define SPI_MOSI_GPIO_Pin		    GPIO_Pin_5

//---------------SPI端口操作预定义-------------------------------------------------
#define AD_SYNC_0()					GPIO_ResetBits(AD7175_SYNC_Port, AD7175_SYNC_Pin)
#define AD_SYNC_1()					GPIO_SetBits(	 AD7175_SYNC_Port, AD7175_SYNC_Pin)

#define SPI_PB_0()					GPIO_ResetBits(SPI_PB_GPIO_Port, SPI_PB_GPIO_Pin)
#define SPI_PB_1()					GPIO_SetBits(	 SPI_PB_GPIO_Port, SPI_PB_GPIO_Pin)

#define SPI_CS_0()					GPIO_ResetBits(SPI_CS_GPIO_Port, SPI_CS_GPIO_Pin)
#define SPI_CS_1()					GPIO_SetBits(	 SPI_CS_GPIO_Port, SPI_CS_GPIO_Pin)
//#define SPI_CS_0()					CAT9555_WriteOneBit(0x00,11,0);
//#define SPI_CS_1()					CAT9555_WriteOneBit(0x00,11,1);

#define SPI_CLK_0()					GPIO_ResetBits(SPI_CLK_GPIO_Port, SPI_CLK_GPIO_Pin)
#define SPI_CLK_1()					GPIO_SetBits(	 SPI_CLK_GPIO_Port, SPI_CLK_GPIO_Pin)

#define SPI_MOSI_0()				GPIO_ResetBits(SPI_MOSI_GPIO_Port, SPI_MOSI_GPIO_Pin)
#define SPI_MOSI_1()				GPIO_SetBits(	 SPI_MOSI_GPIO_Port, SPI_MOSI_GPIO_Pin)

#define Read_SPI_MISO()			GPIO_ReadInputDataBit(SPI_MISO_GPIO_Port, SPI_MISO_GPIO_Pin)

//------------驱动函数声明--------------------------------------------------------------
void SPI_By_IO_GPIO_Init(void);
void SPI_Send_One_Byte(u8 data);
u8 SPI_Read_One_Byte(void);

#endif

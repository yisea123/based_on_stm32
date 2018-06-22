#ifndef __SPI_By_IO_H
#define __SPI_By_IO_H
#include "stm32f10x.h"
extern void delay_us(u32 nus);          //�����ⲿ��ʱ����
extern void delay_ms(u16 nms);
//-------SPI�˿�Ԥ����----------------------------------
#define SPI_CLK_Delay()         Delayus(1)
//#define SPI_CLK_Delay()         Delayms(1)
#define SPI_GPIO_Port_RCC           RCC_APB2Periph_GPIOA
#define SPI_CS_GPIO_Port            GPIOA
#define SPI_CS_GPIO_Pin             GPIO_Pin_4
#define SPI_CLK_GPIO_Port           GPIOA
#define SPI_CLK_GPIO_Pin            GPIO_Pin_3
#define SPI_MISO_GPIO_Port          GPIOA
#define SPI_MISO_GPIO_Pin           GPIO_Pin_6//read     
#define SPI_MOSI_GPIO_Port          GPIOA
#define SPI_MOSI_GPIO_Pin           GPIO_Pin_5
//---------------SPI�˿ڲ���Ԥ����-------------------------------------------------
#define SPI_CS_0()                  GPIO_ResetBits(SPI_CS_GPIO_Port, SPI_CS_GPIO_Pin)
#define SPI_CS_1()                  GPIO_SetBits(    SPI_CS_GPIO_Port, SPI_CS_GPIO_Pin)
#define SPI_CLK_0()                 GPIO_ResetBits(SPI_CLK_GPIO_Port, SPI_CLK_GPIO_Pin)
#define SPI_CLK_1()                 GPIO_SetBits(    SPI_CLK_GPIO_Port, SPI_CLK_GPIO_Pin)

#define SPI_MOSI_0()                    GPIO_ResetBits(SPI_MOSI_GPIO_Port, SPI_MOSI_GPIO_Pin)
#define SPI_MOSI_1()                    GPIO_SetBits(    SPI_MOSI_GPIO_Port, SPI_MOSI_GPIO_Pin)
#define AMP_GS1_0()             GPIO_ResetBits(AMP_GS1_GPIO_Port, AMP_GS1_GPIO_Pin)
#define AMP_GS1_1()             GPIO_SetBits(    AMP_GS1_GPIO_Port, AMP_GS1_GPIO_Pin)
#define AMP_GS0_0()             GPIO_ResetBits(AMP_GS0_GPIO_Port, AMP_GS0_GPIO_Pin)
#define AMP_GS0_1()             GPIO_SetBits(    AMP_GS0_GPIO_Port, AMP_GS0_GPIO_Pin)
#define Read_SPI_MISO()         GPIO_ReadInputDataBit(SPI_MISO_GPIO_Port, SPI_MISO_GPIO_Pin)
//------------������������--------------------------------------------------------------
void SPI_By_IO_GPIO_Init(void);
void SPI_Send_One_Byte(u8 data);
u8 SPI_Read_One_Byte(void);
#endif

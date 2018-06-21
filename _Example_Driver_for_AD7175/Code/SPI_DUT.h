#ifndef __SPI_DUT_H
#define __SPI_DUT_H

#define SPI2_CLK_Delay()         Delayus(1)
//#define SPI_CLK_Delay()         Delayms(1)

#define SPI2_GPIO_Port_RCC           RCC_APB2Periph_GPIOB
#define SPI2_CS_GPIO_Port            GPIOB
#define SPI2_CS_GPIO_Pin             GPIO_Pin_12
#define SPI2_CLK_GPIO_Port           GPIOB
#define SPI2_CLK_GPIO_Pin            GPIO_Pin_13
#define SPI2_MISO_GPIO_Port          GPIOB
#define SPI2_MISO_GPIO_Pin           GPIO_Pin_14//read     
#define SPI2_MOSI_GPIO_Port          GPIOB
#define SPI2_MOSI_GPIO_Pin           GPIO_Pin_15
//---------------SPI端口操作预定义-------------------------------------------------
#define SPI2_CS_0()                  GPIO_ResetBits(SPI2_CS_GPIO_Port, SPI2_CS_GPIO_Pin)
#define SPI2_CS_1()                  GPIO_SetBits(SPI2_CS_GPIO_Port, SPI2_CS_GPIO_Pin)
#define SPI2_CLK_0()                 GPIO_ResetBits(SPI2_CLK_GPIO_Port, SPI2_CLK_GPIO_Pin)
#define SPI2_CLK_1()                 GPIO_SetBits(SPI2_CLK_GPIO_Port, SPI2_CLK_GPIO_Pin)
#define SPI2_MOSI_0()            GPIO_ResetBits(SPI2_MOSI_GPIO_Port, SPI2_MOSI_GPIO_Pin)
#define SPI2_MOSI_1()            GPIO_SetBits(SPI2_MOSI_GPIO_Port, SPI2_MOSI_GPIO_Pin)

#define Read_SPI2_MISO()         GPIO_ReadInputDataBit(SPI2_MISO_GPIO_Port, SPI2_MISO_GPIO_Pin)
//------------驱动函数声明--------------------------------------------------------------
extern void SPI_Write_DUT(uint8_t* data,uint8_t bytesNumber);
extern void SPI_Read_DUT(uint8_t* data,uint16_t bytesNumber);
extern void SPI2_DUT_Init(void);
extern void SPI2_Send_One_Byte(u8 data);
extern u8 SPI2_Read_One_Byte(void);
extern void SPI2_Send_One_Byte_CS(uint8_t data);
#endif



#include "spi.h"

void spi_gpioConfig(void)
{
    GPIO_InitTypeDef SPI1_GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    SPI1_GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7;
    SPI1_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                            //Init IO port to AF output
    SPI1_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    SPI1_GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;                                  //MISO
    SPI1_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    SPI1_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

    GPIO_Init(GPIOA, &SPI1_GPIO_InitStructure);
}

void spi_Config(void)
{
	SPI_InitTypeDef SPI_InitStruction;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
    
    SPI_InitStruction.SPI_Direction = SPI_Direction_2Lines_FullDuplex;              
    SPI_InitStruction.SPI_Mode = SPI_Mode_Master;
    SPI_InitStruction.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStruction.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStruction.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStruction.SPI_NSS = SPI_NSS_Hard;
    SPI_InitStruction.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    SPI_InitStruction.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStruction.SPI_CRCPolynomial = 7;
    
    SPI_Init(SPI1, &SPI_InitStruction);
}

void spi_Init(void)
{
    spi_gpioConfig();
    spi_Config();
    
//    SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_TXE | SPI_I2S_IT_RXNE, ENABLE);
     
    SPI_Cmd(SPI1, ENABLE);
    
}

uint8_t spi_SendByte(uint8_t byte)
{
    assert_param(byte > 0);
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
    {
        SPI_I2S_SendData(SPI1, byte);
        SPI_I2S_ClearITPendingBit(SPI1, SPI_I2S_FLAG_TXE);
    }
    return true;
}

uint8_t spi_ReceiveByte(void)
{
    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
    return(SPI_I2S_ReceiveData(SPI1));
}















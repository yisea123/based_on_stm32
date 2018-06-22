#include "I2C.h"

void I2C_RCC_Configure(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
}
void I2C_GPIO_Configure(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void I2C_Configure(void)
{
    I2C_RCC_Configure();
    I2C_GPIO_Configure();
    
    I2C_InitTypeDef I2C_InitSturctuion;
    
    I2C_InitSturctuion.I2C_ClockSpeed = 10000;
    I2C_InitSturctuion.I2C_Mode = I2C_Mode_SMBusHost;
    I2C_InitSturctuion.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitSturctuion.I2C_OwnAddress1 = DeviceAddress;
    I2C_InitSturctuion.I2C_Ack = I2C_Ack_Enable;
    I2C_InitSturctuion.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    
    I2C_Init(I2C1,&I2C_InitSturctuion);
    
}



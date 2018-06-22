#ifndef __I2C_H
#define __I2C_H

#include "stm32f10x_conf.h"
//#include "stm32f10x_i2c.h"

#define DeviceAddress 0xa0




#define IIC_SCL_PIN         GPIO_Pin_6
#define IIC_SDA_PIN         GPIO_Pin_7

#define IIC_SCL_GPIO        GPIOB
#define IIC_SDA_GPIO        GPIOB

#define __IIC_SCL_SET()     GPIO_WriteBit(IIC_SCL_GPIO, IIC_SCL_PIN, Bit_SET)
#define __IIC_SCL_CLR()     GPIO_WriteBit(IIC_SCL_GPIO, IIC_SCL_PIN, Bit_RESET)

#define __IIC_SDA_SET()		GPIO_WriteBit(IIC_SDA_GPIO, IIC_SDA_PIN, Bit_SET)
#define __IIC_SDA_CLR()     GPIO_WriteBit(IIC_SDA_GPIO, IIC_SDA_PIN, Bit_RESET)

#define __IIC_SDA_IN()     	do { \
								GPIO_InitTypeDef tGPIO; \
								tGPIO.GPIO_Pin = IIC_SDA_PIN; \
								tGPIO.GPIO_Speed = GPIO_Speed_50MHz; \
								tGPIO.GPIO_Mode = GPIO_Mode_IPU; \
								GPIO_Init(IIC_SDA_GPIO, &tGPIO); \
							}while(0)				

#define __IIC_SDA_OUT() 	do { \
								GPIO_InitTypeDef tGPIO; \
								tGPIO.GPIO_Pin = IIC_SDA_PIN; \
								tGPIO.GPIO_Speed = GPIO_Speed_50MHz; \
								tGPIO.GPIO_Mode = GPIO_Mode_Out_PP; \
								GPIO_Init(IIC_SDA_GPIO, &tGPIO); \
							}while(0)   





void I2C_RCC_Configure(void);
void I2C_GPIO_Configure(void);
void I2C_Configure(void);



#endif


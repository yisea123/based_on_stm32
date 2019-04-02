/*
 * i2c.h
 *
 *  Created on: Jan 25, 2019
 *      Author: QUAN
 */

#ifndef I2C_H_
#define I2C_H_

#include "main.h"
#include "stm32f4xx_hal.h"

#define I2C1_SDA_Pin                        GPIO_PIN_9
#define I2C1_SCL_Pin                        GPIO_PIN_8

#define I2C1_Port                  			GPIOB

extern I2C_HandleTypeDef 	hi2c1;
extern I2C_HandleTypeDef 	hi2c2;

DMA_HandleTypeDef hdma_i2c1_rx;
DMA_HandleTypeDef hdma_i2c1_tx;

void MX_I2C1_Init(uint32_t i2c_speed);
void MX_I2C2_Init(uint32_t i2c_speed);


#endif /* I2C_H_ */

/*
 * chip_tmp112.c
 *
 *  Created on: Apr 3, 2019
 *      Author: shush
 */

#include "chip_tmp112.h"

ErrorStatus tmp112_init(void)
{
	HAL_StatusTypeDef status;
	uint8_t buff[] = { config_reg, rate_1Hz };
	status = HAL_I2C_Master_Transmit(&hi2c1, chip_address, buff, 2, 100);
	if (HAL_OK != status)
		return ERROR;
	else
		return SUCCESS;
}

int temp_transfer(unsigned int degree)
{
	uint32_t clear_flag = 0x0fff;
	degree = degree & clear_flag;
	return (800 * (0.0625 / degree));
}
void temp_config(tmp112_address addr, unsigned char data)
{
	HAL_StatusTypeDef status;
	uint8_t
	config_addr[] = {
		[0] = TMP_CONTROL_REG, [1] = 0x00,
	};
	HAL_I2C_Master_Transmit(&hi2c1, TMP_CONTROL_REG, buff, 2, 30);
}

uint16_t temp_read_temp(unsigned char address)
{
	uint32_t tmp;
	HAL_I2C_Master_Receive();
	HAL_StatusTypeDef status;
	uint8_t buff[2] = { [0] = chip_read_addr, [1] =temp_high_reg };
	status = HAL_I2C_Master_Receive((&hi2c1, chip_address, buff, 2, 100);
			tmp = hi2c1->pBuffPtr[0];
			buff[0] = chip_read_addr;
			buff[1] = temp_low_reg;
		status = HAL_I2C_Master_Receive((&hi2c1, chip_address, buff, 2, 100);
					tmp = tmp << 8
					tmp = hi2c1->pBuffPtr[0];
}

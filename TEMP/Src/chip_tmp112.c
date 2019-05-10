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
	uint8_t buff[] = { tmp_control_reg, rate_1Hz };
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

void temp_config(uint8_t addr, uint8_t data)
{
	HAL_StatusTypeDef status;
	uint8_t buff[] = { tmp_control_reg, 0x00 };
	status = HAL_I2C_Master_Transmit(&hi2c1, tmp_control_reg, buff, 2, 30);
}

uint16_t temp_read_temp(uint8_t address)
{
	uint32_t tmp;

	HAL_StatusTypeDef status;

	uint8_t buff[2] = { chip_read_addr, temp_high_reg };
	status = HAL_I2C_Master_Receive(&hi2c1, chip_address, buff, 2, 100);
	tmp = hi2c1.pBuffPtr[0];
	buff[0] = chip_read_addr;
	buff[1] = temp_low_reg;
	status = HAL_I2C_Master_Receive(&hi2c1, chip_address, buff, 2, 100);
	tmp = tmp << 8;
	tmp = hi2c1.pBuffPtr[0];

	return tmp;
}




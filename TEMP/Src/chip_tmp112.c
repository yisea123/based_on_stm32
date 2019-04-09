/*
 * chip_tmp112.c
 *
 *  Created on: Apr 3, 2019
 *      Author: shush
 */

#include "chip_tmp112.h"

ErrorStatus tmp112_init(void) {
	HAL_StatusTypeDef status;
	uint8_t
	buff[] = { config_reg, rate_1Hz };
	status = HAL_I2C_Master_Transmit(&hi2c1, chip_address, buff, 2, 30);
	if (HAL_OK != status)
		return ERROR;
	else
		return SUCCESS;
}

int temp_transfer(unsigned int degree) {
	uint32_t clear_flag = 0x0fff;
	degree = degree & clear_flag;
	return (800 * (0.0625 / degree));
}

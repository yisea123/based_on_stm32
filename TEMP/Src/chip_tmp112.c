/*
 * chip_tmp112.c
 *
 *  Created on: Apr 3, 2019
 *      Author: shush
 */


#include "chip_tmp112.h"

ErrorStatus
tmp112_init (void)
{
	ErrorStatus status;

	return status;
}

int
temp_transfer(unsigned int degree)
{
	uint32_t clear_flag = 0x0fff;
	degree = degree & clear_flag;
	return (800 * (0.0625 / degree));
}

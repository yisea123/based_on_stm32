/*
 * chip_tmp112.h
 *
 *  Created on: Apr 3, 2019
 *      Author: shush
 */

#ifndef CHIP_TMP112_H_
#define CHIP_TMP112_H_

#include "main.h"
#include "i2c.h"
#include "global_cmd.h"
#include "stm32f1xx_hal.h"

/** 12bit temperatures -----------------------------------------------------*/
#define temp_128_degree				0x7ff
#define temp_100_degree				0x640
#define temp_80_degree				0x500
#define temp_75_degree				0x4b0
#define temp_50_degree				0x320
#define temp_25_degree				0x190
#define temp_0p25_degree			0x004
#define temp_0_degree				0x000
#define temp_neg_0p25_degree		0xffc
#define temp_neg_25_degreee			0xe70
#define temp_neg_55_degree			0xc90

typedef enum {
	mode_12bit = 0, mode_13bit = 1
} tmp112_data_width;

#define rate_p25Hz  0		// CR1 = 0, CR0 = 0
#define rate_1Hz 	1		// CR1 = 0, CR0 = 1
#define rate_4Hz	2		// CR1 = 1, CR0 = 0
#define rate_8Hz	3		// CR1 = 1, CR0 = 1

#define chip_address			0x90
#define chip_read_addr			(chip_address | (1 << 0))

#define tmp_control_reg  		0x00
#define temp_reg_read_only 		(tmp_control_reg | (0 << 0)) 	// P1 = 0, P0 = 0
#define config_reg  			(tmp_control_reg | (1 << 0)),				// P1 = 0, P0 = 1
#define temp_low_reg  			(tmp_control_reg | (2 << 0))			// P1 = 1, P0 = 0
#define temp_high_reg  			(tmp_control_reg | (3 << 0))			// P1 = 1, P0 = 1

ErrorStatus tmp112_init(void);

int temp_transfer(unsigned int degree);
void temp_config(uint8_t addr, uint8_t data);
uint16_t temp_read_temp(unsigned char address);



#endif /* CHIP_TMP112_H_ */

/*
 * chip_tmp112.h
 *
 *  Created on: Apr 3, 2019
 *      Author: shush
 */

#ifndef CHIP_TMP112_H_
#define CHIP_TMP112_H_

#include "main.h"

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


ErrorStatus
tmp112_init (void);

int
temp_transfer(unsigned int degree);

#endif /* CHIP_TMP112_H_ */

/*
 * common.h
 *
 *  Created on: Jan 19, 2019
 *      Author: QUAN
 */

#ifndef COMMON_H_
#define COMMON_H_

#define FREE_RTOS				1

#if FREE_RTOS
	#include "cmsis_os.h"
	#define Delay_Ms(x)				osDelay(x)
#else
	#define Delay_Ms(x)				HAL_Delay(x)
#endif



typedef enum {
	error = 0,
	success = !error
} status_enum;


#endif /* COMMON_H_ */

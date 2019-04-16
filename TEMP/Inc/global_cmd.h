/*
 * gloable_cmd.h
 *
 *  Created on: Apr 3, 2019
 *      Author: shush
 */

#ifndef GLOBAL_CMD_H_
#define GLOBAL_CMD_H_

typedef enum {
	untrigger = 0, triggered = !untrigger,
} trigger_enum;

typedef enum {
	standbye = 0, heating = 1, overheat = 2, self_check_pass = 3,
	err_self_check = 4, err_pwr_voltage = 5
} temp_status;

typedef enum {
	off = 0, on = !off
} gpio_status;

unsigned int trigger_mark;
unsigned int heating_mark;
unsigned int standby_mark;
unsigned int off_mark;
unsigned int over_heart_mark;
unsigned int self_check_mark;
unsigned int error_self_check_mark;
unsigned int trigger_status;
unsigned int key_triggered_times;

#define WARM_TMEP		40

#endif /* GLOBAL_CMD_H_ */

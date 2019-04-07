/*
 * gloable_cmd.h
 *
 *  Created on: Apr 3, 2019
 *      Author: shush
 */

#ifndef GLOABLE_CMD_H_
#define GLOABLE_CMD_H_

typedef enum {
	un_trigger = 0, triggered = !un_trigger,
} trigger_enum;

typedef enum {
	standbye = 0, heating = 1, overheat = 2, self_check_pass = 3,
	err_self_check = 4, err_pwr_voltage = 5
} blink_mode;

typedef enum {
	off = 0, on = !off
} gpio_status;

static unsigned int trigger_mark;
static unsigned int heating_mark;
static unsigned int standby_mark;
static unsigned int off_mark;
static unsigned int over_heart_mark;
static unsigned int self_check_mark;
static unsigned int error_self_check_mark;

#endif /* GLOABLE_CMD_H_ */

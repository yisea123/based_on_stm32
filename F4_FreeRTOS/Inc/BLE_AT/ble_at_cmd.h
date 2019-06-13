/*
 * ble_at_cmd.h
 *
 *  Created on: Jun 12, 2019
 *      Author: QUAN
 */

#ifndef BLE_AT_BLE_AT_CMD_H_
#define BLE_AT_BLE_AT_CMD_H_
#include <stdlib.h>

static unsigned char *cmd_buff[] = {
	[0] = "OK",
	[1] = "AT",
	[2] = "AT+RESET",
	[3] = "AT+VERSION?",
	[4] = "AT+NAME?",
	[5] = "AT+NAME=",
	[6] = "AT+PSWD?",
	[7] = "AT+PSWD=",
	[8] = "AT+UART?",
	[9] = "AT+UART=",
};

unsigned char * union_char_str(unsigned char *str1, unsigned char *str2);
int search_str_in_str(unsigned char *str, unsigned char *buff);

#endif /* BLE_AT_BLE_AT_CMD_H_ */

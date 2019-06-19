/*
 * ble_data_handler.c
 *
 *  Created on: Jun 12, 2019
 *      Author: QUAN
 */


#include "ble_at_cmd.h"
unsigned char * union_char_str(unsigned char *str1, unsigned char *str2)
{
	unsigned char *buff;
	unsigned char *buff_head;

	if(NULL == str1)
		return str2;
	if(NULL == str2)
		return str1;

	buff_head = buff = NULL;
	while(*str1 != '\0') {
		buff = malloc(sizeof(unsigned char));
		if(buff) {
			*buff = *str1;
		} else
			return NULL;
		str1++;
		buff++;
	}
	while(*str2 != '\0') {
			buff = malloc(sizeof(unsigned char));
			if(buff) {
				*buff = *str2;
			} else
				return NULL;
			str2++;
			buff++;
	}
	return buff_head;
}

int search_str_in_str(unsigned char *in, unsigned char *find)
{
	int found_flag = 0;
	if(!in || !find) {
		return -1;
	}
	while(NULL != in) {
		if(*find == *in && '\0' != *find) {
			find++;
			in++;
			found_flag = 1;
		} else
			found_flag = 0;
	}
	if(found_flag)
		return 0;
	else
		return 1;
}

/*
 * oled.h
 *
 *  Created on: Jan 25, 2019
 *      Author: QUAN
 */

#ifndef OLED_H_
#define OLED_H_

#include <common.h>
#include "i2c.h"
#include <stdlib.h>

#define ssd1306_cmd_addr          ((unsigned char)0x00)
#define ssd1306_data_addr         ((unsigned char)0x40)

#define MAX_WIDTH				 	128
#define MAX_HEIGHT				 	64

typedef struct {
	uint8_t oled_address;
	uint8_t *data_buff;
}i2c1_typedef;
extern i2c1_typedef * i2c1_info;



void oled_mark_dot(uint8_t x, uint8_t y);
void oled_write_horizontal_line(uint8_t line_index, uint8_t start, uint8_t end);
void oled_write_vertical_line(uint8_t vertical_index, uint8_t start, uint8_t end);
void oled_power_on(void);
void oled_init(void);
void oled_clean(void);
void oled_fill(uint8_t data);
void oled_on(void);
void oled_off(void);
void oled_set_pos(uint8_t x, uint8_t y);

int oled_write_cmd(uint8_t cmd);
int oled_write_data(uint8_t data);

void oled_test(void);

#endif /* OLED_H_ */

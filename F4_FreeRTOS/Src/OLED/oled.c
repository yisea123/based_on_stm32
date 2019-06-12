/*
 * oled.c
 *
 *  Created on: Jan 25, 2019
 *      Author: QUAN
 */

#include "oled.h"

#define ssd1306_cmd_addr          ((unsigned char)0x00)
#define ssd1306_data_addr         ((unsigned char)0x40)

#define MAX_WIDTH				 128
#define MAX_HEIGHT				 64

i2c1_typedef * i2c1_info;

void oled_mark_dot(uint8_t x, uint8_t y) {

}
void oled_write_horizontal_line(uint8_t line_index, uint8_t start, uint8_t end) {

}

void oled_write_vertical_line(uint8_t vertical_index, uint8_t start,
		uint8_t end) {

}
void oled_power_on(void) {
	oled_init();
}
void oled_init(void) {
	HAL_Delay(200);
	oled_write_cmd(0xAE);    //display off
	oled_write_cmd(0x20);    //Set Memory Addressing Mode
	oled_write_cmd(0x10); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	oled_write_cmd(0xb0);  //Set Page Start Address for Page Addressing Mode,0-7
	oled_write_cmd(0xc8);    //Set COM Output Scan Direction
	oled_write_cmd(0x00);    //---set low column address
	oled_write_cmd(0x10);    //---set high column address
	oled_write_cmd(0x40);    //--set start line address
	oled_write_cmd(0x81);    //--set contrast control register
	oled_write_cmd(0xc8);    // black light 0x00~0xff
	oled_write_cmd(0xa1);    //--set segment re-map 0 to 127
	oled_write_cmd(0xa6);    //--set normal display
	oled_write_cmd(0xa8);    //--set multiplex ratio(1 to 64)
	oled_write_cmd(0x3F);    //
	oled_write_cmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	oled_write_cmd(0xd3);    //-set display offset
	oled_write_cmd(0x00);    //-not offset
	oled_write_cmd(0xd5); //--set display clock divide ratio/oscillator frequency
	oled_write_cmd(0xf0);    //--set divide ratio
	oled_write_cmd(0xd9);    //--set pre-charge period
	oled_write_cmd(0x22);    //
	oled_write_cmd(0xda);    //--set com pins hardware configuration
	oled_write_cmd(0x12);
	oled_write_cmd(0xdb);    //--set vcomh
	oled_write_cmd(0x20);    //0x20,0.77xVcc
	oled_write_cmd(0x8d);    //--set DC-DC enable
	oled_write_cmd(0x14);    //
	oled_write_cmd(0xaf);    //--turn on oled panel
}

void oled_on(void) {
	oled_write_cmd(0x8d);
	oled_write_cmd(0x14);
	oled_write_cmd(0xaf);
}
void oled_off(void) {
	oled_write_cmd(0x8d);
	oled_write_cmd(0x10);
	oled_write_cmd(0xae);
}
void oled_fill(uint8_t data) {
	unsigned char index, loop;
	for (index = 0; index < 8; index++) {
		oled_write_cmd(0xb0 + index);    //page0-page1
		oled_write_cmd(0x00);     //low column start address
		oled_write_cmd(0x10);     //high column start address
		for (loop = 0; loop < 128; loop++)
			oled_write_data(data);
	}
}
void oled_clean(void) {

}

void oled_set_pos(uint8_t x, uint8_t y) {
	oled_write_cmd(0xb0 + y);
	oled_write_cmd(((x & 0xf0) >> 4) | 0x10);
	oled_write_cmd((x & 0x0f) | 0x01);
}

int oled_write_cmd(uint8_t cmd) {
	uint8_t data_buff[2];
	HAL_StatusTypeDef ret;

	data_buff[0] = 0x00;
	data_buff[1] = cmd;
	portENTER_CRITICAL();
	ret = HAL_I2C_Master_Transmit(&hi2c1, i2c1_info->oled_address, data_buff, 2,
			200);
	portEXIT_CRITICAL();
	return ret;
}

int oled_write_data(uint8_t data) {
	uint8_t data_buff[2];
	HAL_StatusTypeDef ret;

	data_buff[0] = 0x40;
	data_buff[1] = data;

	portENTER_CRITICAL();
	ret = HAL_I2C_Master_Transmit(&hi2c1, i2c1_info->oled_address, data_buff, 2,
			200);
	portEXIT_CRITICAL();
	return ret;
}

void oled_test(void) {
	for (int index = 0; index < 3; index++) {
		oled_fill(0xff);
		Delay_Ms(3000);
		oled_fill(0x00);
		Delay_Ms(3000);
	}
}

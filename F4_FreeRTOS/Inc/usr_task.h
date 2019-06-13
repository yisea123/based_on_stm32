/*
 * usr_task.h
 *
 *  Created on: Jan 20, 2019
 *      Author: QUAN
 */

#ifndef USR_TASK_H_
#define USR_TASK_H_

#include "cmsis_os.h"
#include "main.h"
#include "usart.h"
#include "blink.h"
#include <stdlib.h>
#include <string.h>
#include "w5500_spi_driver.h"


osThreadId 		defaultTaskHandle;
osThreadId 		BLE_TaskHandle;
osThreadId 		show_all_task_TaskHandle;
osThreadId 		Display_Handle;
osThreadId 		LED_BlinkHandle;
osThreadId 		W5500_Handle;

void StartDefaultTask(void const * argument);
void LED_Blink_Entry(void const * argument);
void BLE_Task_Entry(void const * argument);
void show_all_task(void const * argument);
void Display_Task_Entry(void const * argument);
void W5500_Task_Entry(void const * argument);


void Usr_TaskCreate(void);

extern void oled_clean(void);
extern void oled_fill(uint8_t data);
extern void oled_on(void);
extern void oled_off(void);
extern void oled_set_pos(uint8_t x, uint8_t y);
extern int oled_write_cmd(uint8_t cmd);
extern int oled_write_data(uint8_t data);
extern void oled_test(void);

extern int uasrtx_send_string(UART_HandleTypeDef * huart, uint8_t *str, uint16_t len);



#endif /* USR_TASK_H_ */

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

osThreadId 		defaultTaskHandle;
osThreadId 		BLE_TaskHandle;
osThreadId 		USART_TaskHandle;
osThreadId 		Display_TaskHandle;
osThreadId 		LED_BlinkHandle;

void StartDefaultTask(void const * argument);
void LED_Blink_Entry(void const * argument);
void BLE_Task_Entry(void const * argument);
void USART_Task_Entry(void const * argument);
void Display_Task_Entry(void const * argument);

void Usr_TaskCreate(void);







#endif /* USR_TASK_H_ */

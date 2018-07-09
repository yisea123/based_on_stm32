#ifndef __TASK_CREAT_H
#define __TASK_CREAT_H

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

#include "usart.h"
#include "string.h"
#include "stm32f4xx_hal_def.h"
#include "stm32f4xx_hal_uart.h"

uint8_t signal_buff[100];

/* Variables -----------------------------------------------------------------*/


/* Function prototypes -------------------------------------------------------*/
void LED_blink_task3_Entry(void const * argument);
void usart3_task_Entry(void const * argument);
void sys_info_task_Entry(void const * argument);
/* USER CODE END Application */

/**------------------------------- END OF FILE --------------------------------*/









#endif

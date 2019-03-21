/*
 * usr_task.c
 *
 *  Created on: Jan 20, 2019
 *      Author: QUAN
 */
#include "usr_task.h"

void Usr_TaskCreate(void)
{
	/* Create the thread(s) */
	/* definition and creation of defaultTask */
	osThreadDef(defaultTask, StartDefaultTask, osPriorityBelowNormal, 0, 128);
	defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

	/* definition and creation of LED_Blink */
	osThreadDef(LED_Blink, LED_Blink_Entry, osPriorityBelowNormal, 0, 128);
	LED_BlinkHandle = osThreadCreate(osThread(LED_Blink), NULL);

	/* definition and creation of BLE_Task */
	osThreadDef(BLE_Task, BLE_Task_Entry, osPriorityNormal, 0, 128);
	BLE_TaskHandle = osThreadCreate(osThread(BLE_Task), NULL);

	/* definition and creation of USART_Task */
	osThreadDef(USART_Task, USART_Task_Entry, osPriorityNormal, 0, 512);
	USART_TaskHandle = osThreadCreate(osThread(USART_Task), NULL);

	/* definition and creation of Display_Task */
	osThreadDef(Display_Task, Display_Task_Entry, osPriorityAboveNormal, 0, 512);
	Display_Handle = osThreadCreate(osThread(Display_Task), NULL);

	/* definition and creation of W5500 */
	osThreadDef(W5500_Task, W5500_Task_Entry, osPriorityNormal, 0, 512);
	W5500_Handle = osThreadCreate(osThread(W5500_Task), NULL);
}


/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
	/* USER CODE BEGIN 5 */
	/* Infinite loop */
	static int default_loop = 0;
	for(;;)
	{
		if(0 == default_loop % 2 ) {
			HAL_GPIO_WritePin(LED_Red_Port, LED_Red_Pin, GPIO_PIN_SET);
			 osDelay(300);
		} else {
			HAL_GPIO_WritePin(LED_Red_Port, LED_Red_Pin, GPIO_PIN_RESET);
			osDelay(300);
//			taskYIELD();
		}
		default_loop++;
	}
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_LED_Blink_Entry */
/**
* @brief Function implementing the LED_Blink thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_LED_Blink_Entry */
void LED_Blink_Entry(void const * argument)
{
  /* USER CODE BEGIN LED_Blink_Entry */
  /* Infinite loop */
	for(;;)
	{
	  gpiox_on_off(red, on);
	  osDelay(1000);
	  gpiox_on_off(orange, on);
	  osDelay(1000);
	  gpiox_on_off(blue, on);
	  osDelay(1000);
	  gpiox_on_off(green, on);
	  osDelay(1000);
	  gpiox_on_off(red, off);
	  osDelay(1000);
	  gpiox_on_off(orange, off);
	  osDelay(1000);
	  gpiox_on_off(blue, off);
	  osDelay(1000);
	  gpiox_on_off(green, off);
	  osDelay(1000);
	}
  /* USER CODE END LED_Blink_Entry */
}

/* USER CODE BEGIN Header_BLE_Task_Entry */
/**
* @brief Function implementing the BLE_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_BLE_Task_Entry */
void BLE_Task_Entry(void const * argument)
{
  /* USER CODE BEGIN BLE_Task_Entry */
  /* Infinite loop */
	static int ble_loop = 0;
	for(;;)
	{
		if(0 == ble_loop % 2 ) {
			HAL_GPIO_WritePin(LED_Green_Port, LED_Green_Pin, GPIO_PIN_SET);
			osDelay(300);
		} else {
			HAL_GPIO_WritePin(LED_Green_Port, LED_Green_Pin, GPIO_PIN_RESET);
//			osDelay(300);
			taskYIELD();
		}
		ble_loop++;
	}
  /* USER CODE END BLE_Task_Entry */
}

/* USER CODE BEGIN Header_USART_Task_Entry */
/**
* @brief Function implementing the USART_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_USART_Task_Entry */
void USART_Task_Entry(void const * argument)
{
  /* USER CODE BEGIN USART_Task_Entry */
  /* Infinite loop */
	static uint8_t usart_loop = 0;
	uint8_t buff[128]="-------------------------------------------------\r\n";
	uint16_t buff_len = sizeof(buff)/sizeof(uint8_t) - 1;;

	uint8_t taskListBuff[512] = {'\0'};
	uint16_t taskListBuff_len = sizeof(buff)/sizeof(uint8_t) - 1;
	for(;;)
	{
		if(0 == usart_loop % 2 ) {
			gpiox_on_off(blue, on);
			// osDelay(300);
			taskYIELD();
		} else {
			gpiox_on_off(blue, off);
			osDelay(300);
		}
		usart_loop++;

	//		// get task list
		osThreadList(taskListBuff);
		taskListBuff_len = sizeof(taskListBuff)/sizeof(uint8_t) - 1;;
		uasrtx_send_string(&huart3, taskListBuff, taskListBuff_len);
		memset(taskListBuff, '\0', sizeof(taskListBuff));
		uasrtx_send_string(&huart3, buff, buff_len);
		osDelay(2000);
	}
  /* USER CODE END USART_Task_Entry */
}

/* USER CODE BEGIN Header_Display_Task_Entry */
/**
* @brief Function implementing the Display_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Display_Task_Entry */
void Display_Task_Entry(void const * argument)
{
  /* USER CODE BEGIN Display_Task_Entry */
  /* Infinite loop */
	static int display_loop = 0;
	for(;;)
	{
		if(0 == display_loop % 2) {
			gpiox_on_off(orange, on);
			osDelay(300);
			osThreadYield();
		} else {
			gpiox_on_off(orange, off);
			osDelay(300);
		}
//		oled_test();
//		oled_fill(0xf0);
//		osDelay(2000);
	}
  /* USER CODE END Display_Task_Entry */
}

void W5500_Task_Entry(void const * argument)
{
	static int w5500_loop = 0;
	for(;;)
	{
		if(0 == w5500_loop % 2 ) {
			gpiox_on_off(red, on);
			osDelay(500);
		} else {
			gpiox_on_off(red, off);
			osDelay(500);
		}
		w5500_loop++;
	}
}

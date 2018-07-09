#include "task_creat.h"


//osThreadId <==> void *	//define a void * paramter
osThreadId LED_blink_task3;
void * usart3_task;

osThreadId sys_info_task;


void Bsp_Task_Init(void)
{
	// --------------- led task -------------------------------
	osThreadDef(LED_blink_task3, LED_blink_task3_Entry, osPriorityAboveNormal, 2, 512);
	LED_blink_task3 = osThreadCreate(osThread(LED_blink_task3), NULL);
	// --------------- usart3 task -------------------------------
	osThreadDef(usart3_task, usart3_task_Entry, osPriorityHigh, 2, 512);
	usart3_task = osThreadCreate(osThread(usart3_task), NULL);
	// --------------- running information task -------------------------------

	//creat task fun_p_Entery         task_info       stack size    task_param		priority		句柄
	xTaskCreate(sys_info_task_Entry, "Sys_info_task", 1024, 		NULL, 			2, 				NULL);


//	xReturn = xTaskCreate(	prvIdleTask,
//							"IDLE", configMINIMAL_STACK_SIZE,
//							( void * ) NULL,
//							( tskIDLE_PRIORITY | portPRIVILEGE_BIT ),
//							&xIdleTaskHandle ); /*lint !e961 MISRA exception, justified as it is not a redundant explicit cast to all supported compilers. */
//
//	BaseType_t xTaskCreate(	TaskFunction_t pxTaskCode,
//							const char * const pcName,
//							const uint16_t usStackDepth,
//							void * const pvParameters,
//							UBaseType_t uxPriority,
//							TaskHandle_t * const pxCreatedTask ) /*lint !e971 Unqualified char types are allowed for strings and single characters only. */





}

/* LED_Blink_Entry function */
void LED_blink_task3_Entry(void const * argument)
{
  /* USER CODE BEGIN LED_Blink_Entry */
  /* Infinite loop */
  for(;;)
  {
	  memset(signal_buff, '\0', strlen(signal_buff));
	  strcpy(signal_buff, "led blink task3 running\r\n");
	  HAL_UART_Transmit(&huart3, signal_buff, strlen(signal_buff), 20);

	  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
	  osDelay(500);
	  HAL_GPIO_WritePin(GPIOD,LED2_Pin, GPIO_PIN_RESET);
	  osDelay(500);
	  HAL_GPIO_WritePin(GPIOD, LED3_Pin, GPIO_PIN_RESET);
	  osDelay(500);
	  HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET);
	  osDelay(500);

	  HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET);
	  osDelay(500);
	  HAL_GPIO_WritePin(GPIOD, LED3_Pin, GPIO_PIN_SET);
	  osDelay(500);
	  HAL_GPIO_WritePin(GPIOD, LED2_Pin, GPIO_PIN_SET);
	  osDelay(500);
	  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
	  osDelay(2000);

  }
  /* USER CODE END LED_Blink_Entry */
}

/* USER CODE BEGIN Application */
void usart3_task_Entry(void const * argument)
{
	  for(;;)
	  {
		  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
		  osDelay(300);
		  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
		  osDelay(300);
		  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);

		  memset(signal_buff, '\0', strlen(signal_buff));
		  strcpy(signal_buff, (uint8_t *)"usart3_transmit task running\r\n");
		  HAL_UART_Transmit(&huart3, signal_buff, strlen(signal_buff), 20);
		  osDelay(5000);
	  }
}

void sys_info_task_Entry(void const * argument)
{
	while(1)
	{

	}
}


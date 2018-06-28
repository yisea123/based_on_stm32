/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"


/* Variables -----------------------------------------------------------------*/
osThreadId defaultTaskHandle;
osThreadId LED_BlinkHandle;
osThreadId LED_Blink2_Handle;

/* Function prototypes -------------------------------------------------------*/
void StartDefaultTask(void const * argument);
void LED_Blink_Entry(void const * argument);
void LED_BLink2_Entry(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void vApplicationIdleHook(void);
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName);
void vApplicationMallocFailedHook(void);

/* USER CODE BEGIN 2 */
__weak void vApplicationIdleHook( void )
{
   /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
   to 1 in FreeRTOSConfig.h. It will be called on each iteration of the idle
   task. It is essential that code added to this hook function never attempts
   to block in any way (for example, call xQueueReceive() with a block time
   specified, or call vTaskDelay()). If the application makes use of the
   vTaskDelete() API function (as this demo application does) then it is also
   important that vApplicationIdleHook() is permitted to return to its calling
   function, because it is the responsibility of the idle task to clean up
   memory allocated by the kernel to any task that has since been deleted. */
}

__weak void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName)
{
   /* Run time stack overflow checking is performed if
   configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook function is
   called if a stack overflow is detected. */
}

__weak void vApplicationMallocFailedHook(void)
{
   /* vApplicationMallocFailedHook() will only be called if
   configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h. It is a hook
   function that will get called if a call to pvPortMalloc() fails.
   pvPortMalloc() is called internally by the kernel whenever a task, queue,
   timer or semaphore is created. It is also called by various parts of the
   demo application. If heap_1.c or heap_2.c are used, then the size of the
   heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
   FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
   to query the size of free heap space that remains (although it does not
   provide information on how the remaining heap might be fragmented). */
}


void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 4096);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of LED_Blink */
  osThreadDef(LED_Blink, LED_Blink_Entry, osPriorityAboveNormal, 1, 512);
  LED_BlinkHandle = osThreadCreate(osThread(LED_Blink), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  osThreadDef(LED_Blink2, LED_BLink2_Entry, osPriorityAboveNormal, 2, 1024);
  LED_Blink2_Handle = osThreadCreate(osThread(LED_Blink2), NULL);
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
}

/* StartDefaultTask function */
void StartDefaultTask(void const * argument)
{

  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* LED_Blink_Entry function */
void LED_Blink_Entry(void const * argument)
{
  /* USER CODE BEGIN LED_Blink_Entry */
  /* Infinite loop */
  for(;;)
  {
	HAL_GPIO_WritePin(GPIOD, LED3_Pin|LED2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
    osDelay(1000);
	HAL_GPIO_WritePin(GPIOD, LED3_Pin|LED2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
	osDelay(1000);
  }
  /* USER CODE END LED_Blink_Entry */
}

/* USER CODE BEGIN Application */
void LED_BLink2_Entry(void const * argument)
{
	  for(;;)
	  {
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
		osDelay(300);
		HAL_GPIO_WritePin(GPIOD, LED2_Pin, GPIO_PIN_RESET);
		osDelay(300);
		HAL_GPIO_WritePin(GPIOD, LED3_Pin, GPIO_PIN_RESET);
		osDelay(300);
		HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET);
		osDelay(300);

		HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET);
		osDelay(300);
		HAL_GPIO_WritePin(GPIOD, LED3_Pin, GPIO_PIN_SET);
		osDelay(300);
		HAL_GPIO_WritePin(GPIOD, LED2_Pin, GPIO_PIN_SET);
		osDelay(300);
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
		osDelay(300);



	  }
}
     
/* USER CODE END Application */

/*------------------------------- END OF FILE --------------------------------*/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __dsihost_H
#define __dsihost_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "main.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern DSI_HandleTypeDef hdsi;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

extern void _Error_Handler(char *, int);

void MX_DSIHOST_DSI_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ dsihost_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

//
//
///* Includes ------------------------------------------------------------------*/
//#include "timer.h"
//
///* USER CODE BEGIN 0 */
//
///* USER CODE END 0 */
//
//TIM_HandleTypeDef htim3;
//
///* TIM3 init function */
//void MX_TIM3_Init(void)
//{
//  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
//  TIM_SlaveConfigTypeDef sSlaveConfig = {0};
//  TIM_MasterConfigTypeDef sMasterConfig = {0};
//
//  htim3.Instance = TIM3;
//  htim3.Init.Prescaler = 1000;
//  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
//  htim3.Init.Period = 100;
//  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
//  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
//  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_TRIGGER;
//  sSlaveConfig.InputTrigger = TIM_TS_ITR0;
//  if (HAL_TIM_SlaveConfigSynchronization(&htim3, &sSlaveConfig) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
//  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
//  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
//  {
//    Error_Handler();
//  }
//
//}
//
//void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
//{
//
//  if(tim_baseHandle->Instance==TIM3)
//  {
//  /* USER CODE BEGIN TIM3_MspInit 0 */
//
//  /* USER CODE END TIM3_MspInit 0 */
//    /* TIM3 clock enable */
//    __HAL_RCC_TIM3_CLK_ENABLE();
//  /* USER CODE BEGIN TIM3_MspInit 1 */
//
//  /* USER CODE END TIM3_MspInit 1 */
//  }
//}
//
//void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
//{
//
//  if(tim_baseHandle->Instance==TIM3)
//  {
//  /* USER CODE BEGIN TIM3_MspDeInit 0 */
//
//  /* USER CODE END TIM3_MspDeInit 0 */
//    /* Peripheral clock disable */
//    __HAL_RCC_TIM3_CLK_DISABLE();
//  /* USER CODE BEGIN TIM3_MspDeInit 1 */
//
//  /* USER CODE END TIM3_MspDeInit 1 */
//  }
//}
//
///* USER CODE BEGIN 1 */
//
///* USER CODE END 1 */
//
///************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

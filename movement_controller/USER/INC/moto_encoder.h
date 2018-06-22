/**
  *-----------------------------------------------------------------------------
  * @file    moto_encoder.h
  * @author  V
  * @version V1.0
  * @date    27-NOV-2017
  * @brief   This file contains all the functions prototypes 
  *          for the moto_encoder library.
  *------------------------------------------------------------------------------
  */
#ifndef __MOTO_ENCODER_H__
#define __MOTO_ENCODER_H__

#include "stm32f10x.h"

#define ENCODER_TIM_Period 0xFFFF           //timmer period
#define ENCODER_TIM_Prescaler (72 - 1)

#define ENCODER_TIM_GPIO_APBxClock_FUN RCC_APB2PeriphClockCmd
#define ENCODER_TIM_GPIO_CLK RCC_APB2Periph_GPIOB
#define ENCODER_TIM_PORT GPIOB
#define ENCODER_TIM_PIN GPIO_Pin_0



void TIMx_DCmotor_ENCODER_Init(void);

#endif 


/*-------------------------------------- END LINE -----------------------------------------*/

/**
  *-------------------------------------------------------------------------------------------------
  * @file    moto_encoder.c
  * @author  V
  * @version V1.0
  * @date    27-NOV-2017
  * @brief   This file provides all the moto_encoder functions
  *-----------------------------------------------------------------------------------------------------
  */
#include "moto_encoder.h"

static uint32_t CaptureNumber = 0;

static void ENCODER_TIMx_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = ENCODER_TIM_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(ENCODER_TIM_PORT, &GPIO_InitStructure);
}


static void TIMx_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel                   = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIMx_ARR --> TIMxCNT 重新计数
 *                    TIMx_CCR(电平发生变化)
 * 
 * signal duty cycle = (TIMx_ARR +1 ) * clock period
 * 占空比=TIMx_CCR/(TIMx_ARR +1)
 *ENCODER_TIM_CLK
 */
static void ENCODER_TIMx_Configuration(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    /* Timmer configuration ----------------------------------------------*/
    /* set duty cycle: ENCODER_TIM_Period+1 */
    TIM_TimeBaseStructure.TIM_Period        = ENCODER_TIM_Period;
    /* Output FREQ                          = 72MHz/(ENCODER_TIM_Prescaler)/(ENCODER_TIM_Period-1)*/
    TIM_TimeBaseStructure.TIM_Prescaler     = ENCODER_TIM_Prescaler -1 ;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    /* counter rise up mode -------------------------------------------*/
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    /* Init timmer -----------------------------------------------------*/
    TIM_ICInitStructure.TIM_Channel     = TIM_Channel_3;
    TIM_ICInitStructure.TIM_ICPolarity  = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    /* input time clock prescaler --------------------------------------*/
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    /* IC1F=0000 configurate input filter, choose no filter ------------*/
    TIM_ICInitStructure.TIM_ICFilter = 0x00;
    TIM_ICInit(TIM3, &TIM_ICInitStructure);

    /* configurate NVIC ----------------------------*/
    TIMx_NVIC_Configuration();

    /* enable update interrupt, enable CC1IE capture interrupt --------*/
    TIM_ITConfig(TIM3, TIM_IT_CC3, ENABLE);

    /* enable timmer */
    TIM_Cmd(TIM3, ENABLE);
}

/**
  * @brief: TIMx set sa input capture mode.
  */
void TIMx_DCmotor_ENCODER_Init(void)
{
    ENCODER_TIMx_GPIO_Config();
    ENCODER_TIMx_Configuration();
}

void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_CC3) == SET) //捕获中断
    {
        /* Clear TIM3 Capture compare interrupt pending bit */
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);
        CaptureNumber++;                        //add every rising pulse
    }
}

/*-------------------------------------- END LINE -----------------------------------------*/

#include "timmer.h"

void CounterConfig(void)
{
    /** TIM4  5s -------------------------------------------------*/
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitConfig;
    NVIC_InitTypeDef NVIC_InitConfig;

    TIM_DeInit(TIM4);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    TIM_TimeBaseInitConfig.TIM_Prescaler     = 36000;                //TIM4_CLOCK            = 9MHZ / 9000 = 1KHZ
    TIM_TimeBaseInitConfig.TIM_Period        = (5000 - 1);          //Interrupt time = 5000 / 1KHZ  = 5s
    TIM_TimeBaseInitConfig.TIM_ClockDivision = TIM_CKD_DIV1;        //TIM4_CLINT_CLOCK        = 36MHZ /4     = 9MHZ
    TIM_TimeBaseInitConfig.TIM_CounterMode   = TIM_CounterMode_Up;  //Counter Mode from 0 to Period
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitConfig);

    NVIC_InitConfig.NVIC_IRQChannel                   = TIM4_IRQn;
    NVIC_InitConfig.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitConfig.NVIC_IRQChannelSubPriority        = 1;
    NVIC_InitConfig.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitConfig);

    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE); //Update TIM interrupt
    TIM_ClearFlag(TIM4, TIM_IT_Update);
   
  



    /** TIM2  1s -------------------------------------------------*/
    TIM_DeInit(TIM2);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_TimeBaseInitConfig.TIM_Prescaler = 36000;                //TIM4_CLOCK   = 72MHZ/ 36000   = 2KHZ
    TIM_TimeBaseInitConfig.TIM_Period = (2000 - 1);             //Interrupt time   = 10000 / 2KHZ = 5s
    TIM_TimeBaseInitConfig.TIM_ClockDivision = TIM_CKD_DIV1;     //TIM4_CLINT_CLOCK = 72MHZ /1     = 72MHZ
    TIM_TimeBaseInitConfig.TIM_CounterMode = TIM_CounterMode_Up; //Counter Mode from 0 to Period
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitConfig);

    NVIC_InitConfig.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitConfig.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitConfig.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitConfig.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitConfig);

    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); //Update TIM interrupt
    TIM_ClearFlag(TIM2, TIM_IT_Update);

    /** TIM6  5s -------------------------------------------------*/

//    TIM_DeInit(TIM6);

//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

//    TIM_TimeBaseInitConfig.TIM_Prescaler = 7200;                //TIM6_CLOCK            = 9MHZ / 9000 = 1KHZ
//    TIM_TimeBaseInitConfig.TIM_Period = (50000 - 1);              //Interrupt time = 5000 / 1KHZ  = 5s
//    TIM_TimeBaseInitConfig.TIM_ClockDivision = TIM_CKD_DIV1;     //TIM6_CLINT_CLOCK        = 36MHZ /4     = 9MHZ
//    TIM_TimeBaseInitConfig.TIM_CounterMode = TIM_CounterMode_Up; //Counter Mode from 0 to Period
//    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitConfig);

//    NVIC_InitConfig.NVIC_IRQChannel = TIM6_IRQn;
//    NVIC_InitConfig.NVIC_IRQChannelPreemptionPriority = 2;
//    NVIC_InitConfig.NVIC_IRQChannelSubPriority = 2;
//    NVIC_InitConfig.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitConfig);

//    TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE); //Update TIM interrupt
//    TIM_ClearFlag(TIM6, TIM_IT_Update);
}

void HeartBeat(FunctionalState NewState)
{
    if(NewState == ENABLE)
    {
        TIM_Cmd(TIM4, ENABLE); //Open TIM4
    }
    else
    {
        TIM_Cmd(TIM4, DISABLE); //Open TIM4
    }
}
void Timmer_TIM2(FunctionalState NewState)
{
    if (NewState == ENABLE)
    {
        TIM_Cmd(TIM2, ENABLE); //Open TIM2
    }
    else
    {
        TIM_Cmd(TIM2, DISABLE); //Open TIM2
    }
}
void SystemInitSuccessTimmer(FunctionalState NewState)
{
    if (NewState == ENABLE)
    {
        TIM_Cmd(TIM6, ENABLE); //Open TIM2
    }
    else
    {
        TIM_Cmd(TIM6, DISABLE); //Open TIM2
    }
}

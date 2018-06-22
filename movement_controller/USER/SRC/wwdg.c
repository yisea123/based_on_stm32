/**
  *-------------------------------------------------------------------------------------------------
  * @file    iwdg.c
  * @author  V
  * @version V1.0
  * @date    27-NOV-2017
  * @brief   This file provides all the iwdg functions
  *-----------------------------------------------------------------------------------------------------
  */

#include "wwdg.h"

/**
 * @brief: watch dog adopt protocols
 */
void DogAdopt(void)
{
    WWDG_DeInit();
    NVIC_InitTypeDef    NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);        
    WWDG_SetPrescaler(WWDG_Prescaler_8);
    WWDG_SetWindowValue(80);
    WWDG_Enable(127);
    WWDG_ClearFlag();

    /** WWDG NVIC ----------------------------------------------------*/
    NVIC_InitStructure.NVIC_IRQChannel                   = WWDG_IRQn;       
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 3;       
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure); 

    WWDG_EnableIT();
}

void WWDG_IRQHandler(void)
{
    WWDG_SetCounter(0x7f);
    WWDG_ClearFlag();
}

/*-------------------------------------- END LINE -----------------------------------------*/

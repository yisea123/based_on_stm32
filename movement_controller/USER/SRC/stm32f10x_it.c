
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"

uint8_t timeCounter  =  0;
float motorCurrentValueTemp = 0;
/**
  * @brief  This function TIM1_IRQ exception.
  * @param  None
  * @retval None
  */
void TIM1_UP_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
    {
        pulseCounter++;
    }
    if (pulseCounter == 65500)
    {
       pulseCounter = 0;
    }
    #ifdef AC200_DEBUG
        printf("pulse = %d\r\n", pulseCounter);
    #endif
    TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
}

/**
  * @brief  This function TIM4_IRQ exception.
  * @brief  CAN heart beat fixed in 1 second.
  * @param  None
  * @retval None
  */
void TIM4_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) == SET)
    {
        LED2_ON();     
        timeCounter++;        
    }
   
    if (timeCounter == 5)
    {
      /** can heart beat ---------- 5s per time ---------------*/
        CAN_SendMessage.CAN_Standard_ID_No = AC200_TICK_ID;
        CAN_SendMessage.CAN_SendDataLength = 8;
        CAN_SendMessage.CAN_DATA[0]        = AC_200_StatusNumber;
        CAN_SendMessage.CAN_DATA[1]        = 0x00;
        CAN_SendMessage.CAN_DATA[2]        = 0x00;
        CAN_SendMessage.CAN_DATA[3]        = 0x00;
        CAN_SendMessage.CAN_DATA[4]        = 0x00;
        CAN_SendMessage.CAN_DATA[5]        = 0x00;
        CAN_SendMessage.CAN_DATA[6]        = 0x00;
        CAN_SendMessage.CAN_DATA[7]        = 0x00;
        CAN_Send_Message(&CAN_SendMessage);
        /** can heart beat end -------------------------*/
		    LED2_OFF();
        timeCounter = 0;
        #ifdef AC200_DEBUG
          printf("\r\nHeart Beat...\r\n");
        #endif
    }
    TIM4 ->CNT = 0;
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
}

/**
  * @brief  This function TIM6_IRQ exception.
  * @param  None
  * @retval None
  */
//void TIM6_UP_IRQHandler(void)
//{
////    if (TIM_GetITStatus(TIM6, TIM_IT_Update) == SET)
////    {
////        pulseCounter++;
////    }
////    if (pulseCounter == 65500)
////    {
////       pulseCounter = 0;
////    }
////    #ifdef AC200_DEBUG
////        printf("pulse = %d\r\n", pulseCounter);
////    #endif
//    TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
//}
/**
  * @brief  This function TIM2_IRQ. for motor ADC sample timmer.
  * @param  None
  * @retval None
  */
void TIM2_IRQHandler(void)
{
    DC_MOTOR_RunStatus.DC_MOTOR_CurrentProtection = OFF;
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
    {
      motorCurrentValueTemp = GetMotorCurrentOriginal(2);
      if((motorCurrentValueTemp > 3) || (motorCurrentValueTemp < 1))
      {
          DC_MOTOR_PWM(DISABLE);
          DC_MOTOR_Ready(DISABLE);
          DC_MOTOR_RunStatus.DC_MOTOR_CurrentProtection = ON;
      }
      TIM2 ->CNT = 0;
    }
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}

/**
  * @brief  This function Current protection trigger.
  * @param  None
  * @retval None
  */
void EXTI9_5_IRQHandler(void)
{
    DelayMs(10);
    if (EXTI_GetITStatus(EXTI_Line8) == SET)
    {

        DC_MOTOR_Ready(DISABLE);
        DC_MOTOR_Periph(DISABLE);
      #ifdef AC200_DEBUG
        printf("\r\nCurrent1 EXTI succeed..\r\n");
      #endif
      EXTI_ClearITPendingBit(EXTI_Line8);
    }
    else if(EXTI_GetITStatus(EXTI_Line9) == SET)
    {

        DC_MOTOR_Ready(DISABLE);
        DC_MOTOR_Periph(DISABLE);
      #ifdef AC200_DEBUG
        printf("\r\nCurrent2 EXTI succeed..\r\n");
      #endif
      EXTI_ClearITPendingBit(EXTI_Line9);
    }
}
/*--------------- motor position trigger EXTI_Handler start ----------------*/
/**
  * @brief  This function motor high position trigger exti.
  * @param  None
  * @retval None
  */
void EXTI15_10_IRQHandler(void)
{
    DelayMs(10);
    if (EXTI_GetITStatus(EXTI_Line13) == SET)
    {
        DC_MOTOR_Position.PositonTrigger_High = ON;
        #ifdef AC200_DEBUG
            printf("\r\nmotor position high tirgger succeed..\r\n");
        #endif
    }
    EXTI_ClearITPendingBit(EXTI_Line13);
}
/**
  * @brief  This function motor low position trigger exti.
  * @param  None
  * @retval None
  */
void EXTI1_IRQHandler(void)
{
    DelayMs(10);
    if (EXTI_GetITStatus(EXTI_Line2) == SET)
    {
        DC_MOTOR_Position.PositonTrigger_Low = ON;
        #ifdef AC200_DEBUG
            printf("\r\nmotor position low tirgger succeed..\r\n");
        #endif
    }
    EXTI_ClearITPendingBit(EXTI_Line2);
}
/*-------------- motor position trigger EXTI_Handler end ----------------*/
/*-----------------------------------------------------------------------*/
/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}



/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

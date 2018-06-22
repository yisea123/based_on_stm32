#include "system_config.h"

void NVIC_GropConfig(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
}


///** System Clock Configuration
//*/
//void SystemClock_Config(void)
//{

//  RCC_OscInitTypeDef RCC_OscInitStruct;
//  RCC_ClkInitTypeDef RCC_ClkInitStruct;

//    /**Initializes the CPU, AHB and APB busses clocks 
//    */
//  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
//  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
//  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
//  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
//  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
//  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
//  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
//  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
//  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
//  {
//    Error_Handler();
//  }

//    /**Initializes the CPU, AHB and APB busses clocks 
//    */
//  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
//                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
//  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
//  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV8;
//  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
//  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

//  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
//  {
//    Error_Handler();
//  }

//    /**Configure the Systick interrupt time 
//    */
//  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

//    /**Configure the Systick 
//    */
//  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

//  /* SysTick_IRQn interrupt configuration */
//  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
//}










///*************************************************************************************************************************

//* �������������о���֪�������񱻶�·�Ժ󣬽���NMI�жϡ�����жϲ���Ҫ���������ҵȼ��ܺá�  
//*        ��Ʒ���������CSS����-����RCC�жϵȼ�  
//*                                              |  
//*                                              |  
//*                                              |  
//*                                          NMI�ж���-------��HSE--����HSE�жϾ����ж�  
//*                                          |  
//*                                          |  
//*                                          RCC�ж�-------���HSE����״̬----����HSE��Ϊʱ��----�ر�HSE�����ж�----���״̬λ
//**************************************************************************************************************************/

// /**************************RCC�жϴ�����**************************/  
////��HSE�ָ�����ʱ����ú���  
//void RCC_IRQHandler(void)  
//    {  
//      
//          
//        if(RESET != RCC_GetITStatus(RCC_IT_HSERDY))                                               //���HSE����  
//        {  
//              
//        #if defined (DBGUG_RCC_IRQHandler)  
//              
//            printf("i'm in IRQ\n");                                                               //�����������RCC�ж�  
//              
//        #endif    
//          
//            Set_SysClockToHSE();                                                                  //����ϵͳʱ��ΪHSE  
//            RCC_ITConfig(RCC_IT_HSERDY, DISABLE);                                                 //�ر�HSE�����ж�   
//            RCC_ClearITPendingBit(RCC_IT_HSERDY);                                                 // ���HSE�����жϱ�־λ  
//              
//  
//        }  
//          
//          
//    } 

//    /*************************NMI�жϺ������**************************/  
//    //���ⲿ���񱻶�·������������NMI�жϺ���  
//      
//      
//     void NMI_Handler(void)  
//     {  
//           
//         if (RCC_GetITStatus(RCC_IT_CSS) != RESET)                                                //HSE��PLl�Ѿ�����ֹ(����PLL����δ��)   
//         {  
//               
//         #if defined(DEBUG_NMI_Handler)  
//               
//             printf("i'm in NMI\n");                                                              //�����������NMI�ж�  
//               
//         #endif  
//           
//             RCC_HSEConfig(RCC_HSE_ON);                                                           // ʹ��HSE  
//             RCC_ITConfig(RCC_IT_HSERDY, ENABLE);                                                 // ʹ��HSE�����ж�   
//               
//             RCC_ClearITPendingBit(RCC_IT_CSS);                                                   // ���ʱ�Ӱ�ȫϵͳ���жϹ���λ  
//              
//         }  
//      
//           
//    }  

//void CSS_Init(void)  
//{  
//    //����HSE�����жϵ����ȼ�  
//    //NVIC ����  
//    NVIC_InitTypeDef NVIC_InitStructure;  
//      
//    NVIC_InitStructure.NVIC_IRQChannel = RCC_IRQn;  
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;                              //��ռ���ȼ�0  
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                                    //�����ȼ�0  
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                       //IRQͨ��ʹ��  
//    NVIC_Init(&NVIC_InitStructure);                                                       //����ָ���Ĳ�����ʼ��VIC�Ĵ���  
//      
//    RCC_ClockSecuritySystemCmd(ENABLE);   //����ʱ�Ӱ�ȫϵͳCSS  
//}

//    /************************************************************* 
//    ****�� �� ����Set_SysClockToHSE 
//    ****��    �ܣ�����ϵͳʱ��ΪHSE 
//    ****��ڲ������� 
//    ****�� �� ֵ�� 
//    ****˵    ������ 
//    **************************************************************/    
//          
//          
//    static void Set_SysClockToHSE(void)  
//    {  
//      __IO uint32_t StartUpCounter = 0, HSEStatus = 0;  
//        
//      /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------*/      
//      /* Enable HSE */      
//      RCC->CR |= ((uint32_t)RCC_CR_HSEON);  
//       
//      /* Wait till HSE is ready and if Time out is reached exit */  
//      do  
//      {  
//        HSEStatus = RCC->CR & RCC_CR_HSERDY;  
//        StartUpCounter++;    
//      } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));  
//      
//      if ((RCC->CR & RCC_CR_HSERDY) != RESET)  
//      {  
//        HSEStatus = (uint32_t)0x01;  
//      }  
//      else  
//      {  
//        HSEStatus = (uint32_t)0x00;  
//      }    
//      
//      if (HSEStatus == (uint32_t)0x01)  
//      {  
//      
//    #if !defined STM32F10X_LD_VL && !defined STM32F10X_MD_VL && !defined STM32F10X_HD_VL  
//        /* Enable Prefetch Buffer */  
//        FLASH->ACR |= FLASH_ACR_PRFTBE;  
//      
//        /* Flash 0 wait state */  
//        FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);  
//      
//    #ifndef STM32F10X_CL  
//        FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_0;  
//    #else  
//        if (HSE_VALUE <= 24000000)  
//        {  
//          FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_0;  
//        }  
//        else  
//        {  
//          FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_1;  
//        }  
//    #endif /* STM32F10X_CL */  
//    #endif  
//       
//        /* HCLK = SYSCLK */  
//        RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;  
//            
//        /* PCLK2 = HCLK */  
//        RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;  
//          
//        /* PCLK1 = HCLK */  
//        RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV1;  
//          
//        /* Select HSE as system clock source */  
//        RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));  
//        RCC->CFGR |= (uint32_t)RCC_CFGR_SW_HSE;      
//      
//        /* Wait till HSE is used as system clock source */  
//        while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x04)  
//        {  
//        }  
//      }  
//      else  
//      { /* If HSE fails to start-up, the application will have wrong clock  
//             configuration. User can add here some code to deal with this error */  
//      }    
//    }  










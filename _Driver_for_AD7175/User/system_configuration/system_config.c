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

//* 根据以上理论研究可知，当晶振被短路以后，进入NMI中断。这个中断不需要开启，而且等级很好。  
//*        设计方案：开启CSS功能-设置RCC中断等级  
//*                                              |  
//*                                              |  
//*                                              |  
//*                                          NMI中断中-------打开HSE--开启HSE中断就绪中断  
//*                                          |  
//*                                          |  
//*                                          RCC中断-------检测HSE就绪状态----设置HSE作为时钟----关闭HSE就绪中断----清楚状态位
//**************************************************************************************************************************/

// /**************************RCC中断处理函数**************************/  
////当HSE恢复正常时进入该函数  
//void RCC_IRQHandler(void)  
//    {  
//      
//          
//        if(RESET != RCC_GetITStatus(RCC_IT_HSERDY))                                               //如果HSE就绪  
//        {  
//              
//        #if defined (DBGUG_RCC_IRQHandler)  
//              
//            printf("i'm in IRQ\n");                                                               //串口输出进入RCC中断  
//              
//        #endif    
//          
//            Set_SysClockToHSE();                                                                  //设置系统时钟为HSE  
//            RCC_ITConfig(RCC_IT_HSERDY, DISABLE);                                                 //关闭HSE就绪中断   
//            RCC_ClearITPendingBit(RCC_IT_HSERDY);                                                 // 清除HSE就绪中断标志位  
//              
//  
//        }  
//          
//          
//    } 

//    /*************************NMI中断函数入口**************************/  
//    //当外部晶振被短路不能起振后进入NMI中断函数  
//      
//      
//     void NMI_Handler(void)  
//     {  
//           
//         if (RCC_GetITStatus(RCC_IT_CSS) != RESET)                                                //HSE、PLl已经被禁止(但是PLL设置未变)   
//         {  
//               
//         #if defined(DEBUG_NMI_Handler)  
//               
//             printf("i'm in NMI\n");                                                              //串口输出进入NMI中断  
//               
//         #endif  
//           
//             RCC_HSEConfig(RCC_HSE_ON);                                                           // 使能HSE  
//             RCC_ITConfig(RCC_IT_HSERDY, ENABLE);                                                 // 使能HSE就绪中断   
//               
//             RCC_ClearITPendingBit(RCC_IT_CSS);                                                   // 清除时钟安全系统的中断挂起位  
//              
//         }  
//      
//           
//    }  

//void CSS_Init(void)  
//{  
//    //设置HSE就绪中断的优先级  
//    //NVIC 配置  
//    NVIC_InitTypeDef NVIC_InitStructure;  
//      
//    NVIC_InitStructure.NVIC_IRQChannel = RCC_IRQn;  
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;                              //抢占优先级0  
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                                    //子优先级0  
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                       //IRQ通道使能  
//    NVIC_Init(&NVIC_InitStructure);                                                       //根据指定的参数初始化VIC寄存器  
//      
//    RCC_ClockSecuritySystemCmd(ENABLE);   //启动时钟安全系统CSS  
//}

//    /************************************************************* 
//    ****函 数 名：Set_SysClockToHSE 
//    ****功    能：设置系统时钟为HSE 
//    ****入口参数：无 
//    ****返 回 值： 
//    ****说    明：无 
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










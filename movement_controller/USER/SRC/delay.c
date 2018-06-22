#include "delay.h"


void DelayUs(uint32_t us)
{
	uint32_t temp;
	/**
	 * set systick load number:
	 * load number = (((System Clock) / SysTick_CLKSource_HCLK_Div8) / 9) = 1 us
	 * load value is 24bit number 0xffffff
	 * max delay time using delayUs is 16,777,215/1000,000 = 16 second
	 * */
	SysTick -> LOAD=(u32)us * (9);	//time load 72Mhz/8000/1000 (SysTick->LOAD为24bit)
	SysTick -> VAL =0x00;           		
	SysTick -> CTRL=0x01 ;          		
	do
	{
		temp=SysTick -> CTRL;
	}while((temp & 0x01) && !(temp & (1 << 16)));
	SysTick -> CTRL=0x00;      	 			
	SysTick -> VAL =0X00;       				
}
void DelayMs(uint32_t ms)
{
	uint32_t temp;
	/**
	 * set systick load number:
	 * load number = (((System Clock) / SysTick_CLKSource_HCLK_Div8) / 8000) = 1 ms
	 * load value is 24bit number 0xffffff
	 * */

    for(;ms > 0; ms--)
    {
        SysTick->LOAD = (uint32_t)9000;			// set time load number:
                                                    //		load number = 9Mhz/9000 (SysTick->LOAD为24bit)
        SysTick -> VAL =0x00;           			// empty systick value register
        SysTick -> CTRL=0x01;          			// start counter  
        do
        {
            temp=SysTick -> CTRL;
        }while((temp & 0x01) && !(temp & (1 << 16)));	// waitting for counter
        SysTick -> VAL =0X00;       					// clear systick counter value
    }
	SysTick -> CTRL=0x00;      	 					// close systick timmer
	SysTick -> VAL =0X00;       					// clear systick counter value
}

void Systick_Configuration(void)
{

	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);		//9MHz


	    /* Disable Systick counter */
		SysTick_CounterCmd(SysTick_Counter_Disable);
		/* Disable Systick timmer interrupt */
		SysTick_ITConfig(DISABLE);
	    /**
	     *    SysTick_CLKSource_HCLK_Div8: AHB clock divided by 8 selected as SysTick clock source.
	     *    freq = 72 /8 = 9MHz
	     */
	    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	    SysTick_CounterCmd(SysTick_Counter_Clear);
	    /* Enable Systick timmer interrupt */
	//    SysTick_ITConfig(ENABLE);
	    SysTick_SetReload(9000);	//systick interrupt time  = 9000/8MHz = 1ms
	    /** Start Systick Counter */
	    SysTick_CounterCmd(SysTick_Counter_Enable);	
}

///**
//  * @brief  This function handles SysTick Handler.
//  * @param  None
//  * @retval None
//  */
//void SysTick_Handler(void)
//{
//}



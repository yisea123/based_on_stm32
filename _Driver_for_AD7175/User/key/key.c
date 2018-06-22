
#include "key.h"

uint32_t IncreaseCounterTimes     = 0;
uint32_t FreqRegisterCounterTimes = 0;
uint32_t sbutractCounterTimes     = 0;

uint32_t FrequencyValue = 1000;

key_FlagTypedef key_flag;


void key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruction;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);

	GPIO_InitStruction.GPIO_Pin   = key_increase | key_subtract | key_FreqRegister;
	GPIO_InitStruction.GPIO_Mode  = GPIO_Mode_IPU;
	GPIO_InitStruction.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Key_GPIO, &GPIO_InitStruction);

    key_flag.increase_flag = 0;
    key_flag.sbutract_flag = 0;
    key_flag.FreqRegisterChage_flag = 0;
	
}

//void key_scan(void)
//{

//    if(0 == GPIO_ReadInputDataBit(Key_GPIO, key_increase))
//    {
//        delay_ms(50);
//        if(0 == GPIO_ReadInputDataBit(Key_GPIO, key_increase))
//        {
//            key_flag.increase_flag = 1;
//            for(uint8_t i = 0; i < 3; i++)
//            {
//            	LED_AllSet();
//            	delay_ms(300);
//            	LED_AllDim();
//            	delay_ms(300);
//            }
//            
//        }
//    }
//    if(0 == GPIO_ReadInputDataBit(Key_GPIO, key_subtract))
//    {
//        delay_ms(50);
//        if(0 == GPIO_ReadInputDataBit(Key_GPIO, key_subtract))
//        {
//            key_flag.sbutract_flag = 1;
//            for(uint8_t i = 0; i < 3; i++)
//            {
//            	LED_AllSet();
//            	delay_ms(300);
//            	LED_AllDim();
//            	delay_ms(300);
//            }
//        }
//    }
//    if(0 == GPIO_ReadInputDataBit(Key_GPIO, key_FreqRegister))
//    {
//        delay_ms(50);
//        if(0 == GPIO_ReadInputDataBit(Key_GPIO, key_FreqRegister))
//        {
//            key_flag.FreqRegisterChage_flag = 1;
//            for(uint8_t i = 0; i < 3; i++)
//            {
//            	LED_AllSet();
//            	delay_ms(300);
//            	LED_AllDim();
//            	delay_ms(300);
//            }
//        }
//    }
//	
//	if(key_flag.FreqRegisterChage_flag)
//	{
//		FreqRegisterCounterTimes++;
//		if(0 != (FreqRegisterCounterTimes % 2))		//判断是双数还是单数，单数寄存器1，双数寄存器0.
//		{
//			key_flag.FreqRegisterChage_flag = 1;
//		}
//		else
//		{
//			key_flag.FreqRegisterChage_flag = 0;
//		}
//	}
//	if(key_flag.increase_flag)
//	{
//		IncreaseCounterTimes++;
//        AD9834_WriteBytes(AD9834_InitAddress);
//        delay_ms(2000);
//        FrequencyValue = (FrequencyValue + 500);//(IncreaseCounterTimes * 200));
//		AD9834_OutputSet(key_flag.FreqRegisterChage_flag, FrequencyValue, 0);
//        for(uint8_t i = 0; i < 3; i++)
//        {
//        	LED_AllSet();
//        	delay_ms(300);
//        	LED_AllDim();
//        	delay_ms(300);
//        }
//        key_flag.increase_flag = 0;
//        sbutractCounterTimes = 0;		//减法技术清零
//	}
//	if(key_flag.sbutract_flag)
//	{
//		sbutractCounterTimes++;
//        AD9834_WriteBytes(AD9834_InitAddress);
//        delay_ms(2000);
//        FrequencyValue = (FrequencyValue - 500);//(sbutractCounterTimes * 200));
//        if(FrequencyValue <= 0)				//下限限制
//        {
//        	AD9834_OutputSet(key_flag.FreqRegisterChage_flag, 10, 0);
//        }
//        else
//        {
//        	AD9834_OutputSet(key_flag.FreqRegisterChage_flag, FrequencyValue, 0);
//        }
//        for(uint8_t i = 0; i < 3; i++)
//        {
//        	LED_AllSet();
//        	delay_ms(300);
//        	LED_AllDim();
//        	delay_ms(300);
//        }
//        key_flag.sbutract_flag = 0;
//        IncreaseCounterTimes = 0;		//加法计数清零。
//	}
//}










/************************************* END LINE ************************************************/

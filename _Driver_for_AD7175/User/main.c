
#include "main.h"
int main(void)
{    
    //
//Init RCC_HSE, Init EXTI XTAL
    RCC_DeInit();
    RCC_HSEConfig( RCC_HSE_ON );
    
//Set NVIC Group Priority  
    NVIC_GropConfig();  

//Init Periphery Device
    GPIO_Config();
    
//
    //ExitKeyConfig();
    //Usart_Init();
    key_Init();
//    AD9834_Init();
//    AD9834_OutputSet(FreqRegister_0, 1000, 0);
    blink();
	while(1)
	{
        blink();
//        key_scan();
	}


}


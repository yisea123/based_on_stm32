
#include "main.h"
int main(void)
{    
    //NVIC_GropConfig();
    GPIO_Config();
    //ExitKeyConfig();
    //Usart_Init();
    key_Init();
    AD9834_Init();
    AD9834_OutputSet(FreqRegister_0, 1000, 0);
    blink();
	while(1)
	{
        key_scan();
	}


}


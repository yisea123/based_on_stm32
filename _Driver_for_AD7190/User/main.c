
#include "main.h"
int main(void)
{    
    systemInit();
    GPIO_Config();
    USART_conf();
    SPI1_Init();
    AD7190_Init();
    delay_ms(100);
    AD7190_Reset();
    delay_ms(100);
    CountBlink(2, 3, 200);
	while(1)
	{
        blink();
	}


}

void systemInit(void)
{
    RCC_HSEConfig(RCC_HSE_ON);
    NVIC_GropConfig();
    delay_init(72);             //system clock speed 72MHZ
}

/************************************* END LINE ************************************************/


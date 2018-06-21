/**********************************************************************************/
#include "config.h"
void  Initialize_Device(void)
{
//	Power1V8_On;
//	Power3V3_On;
//	DUT_Detecte_On;
//	TIM3_Int_Init(4999,7199);
	
    Uart1Init(115200);
    SPI_By_IO_GPIO_Init();
    Init_AD7175();
    SPI_AD5660_Init();
//  AT24CXX_Init();
    TMP102_Init();
    init_ad9833();                                                    
	SPI2_DUT_Init();
	output(1000);
	
}
/*
 * 函数名：main
 * 描述  : 主函数
 * 输入  ：无
 * 输出  : 无
 */
int main(void)
{
    float Tem_F;
    SystemInit (); //系统初始化 调用官方库
    SysTick_Config(72000);//Set SysTick to 1ms   SystemCoreClock=8MHZ,72000000/1000=72000  调用官方库
    GPIO_Configuration();
    NVIC_Configuration();
    Delayms(5);
    Initialize_Device();
    printf("ok now");

    while(1)
    {
        UART1_Operation_function();
//		Delayus(10);
    }
}
/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/

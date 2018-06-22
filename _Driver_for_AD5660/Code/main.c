/******************** (C) COPYRIGHT 2012 WildFire Team **************************
 * 文件名  ：main.c
 * 描述    ：用3.5.0版本建的工程模板。
 * 实验平台：野火STM32开发板
 * 库版本  ：ST3.5.0
 *
 * 作者    ：wildfire team
 * 论坛    ：http://www.amobbs.com/forum-1008-1.html
 * 淘宝    ：http://firestm32.taobao.com
**********************************************************************************/
#include "config.h"
void Initialize_Device(void)
{
    Uart1Init(115200);
    SPI_By_IO_GPIO_Init();
	Init_AD7175();
	SPI_AD5660_Init();
}
/*
 * 函数名：main
 * 描述  : 主函数
 * 输入  ：无
 * 输出  : 无
 */
int main(void)
{

    SystemInit (); //系统初始化 调用官方库
    SysTick_Config(72000);//Set SysTick to 1ms   SystemCoreClock=8MHZ,72000000/1000=72000  调用官方库
    GPIO_Configuration();
    NVIC_Configuration();
    Delayms(5);
    Initialize_Device();
	printf("ok now");

    while(1)
    {
//		printf("ok now");
        UART1_Operation_function();
        Delayms(200);
    }
}
/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/

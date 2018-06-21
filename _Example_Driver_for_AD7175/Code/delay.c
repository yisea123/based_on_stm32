//#define DELAY_GLOBALS
#include "config.h"
uint Delay1msCount=0;
uint Delay1ms=0;
uint Delay10ms=0;
uint ItemDelay1ms=0;
int OverTime;
//#include "GlobalDef.h"
/************************
功能:SysTick_Handler dell
版本:V1.00   日期:11.10.06
修改:xiaodong.liu
作者:value
1ms
************************/
void SysTick_Handler(void)
{
    Delay1ms++;
    Delay10ms++;
    ItemDelay1ms++;
    OverTime--;
    if(OverTime<0)
    {
        OverTime=0;
    }
    Delay1msCount--;
    if(Delay10ms<2)
    {
        Delay10ms=0;
//          GPIOA->ODR^= GPIO_Pin_3;     //PE2引脚的电平取反
//          GPIOA->ODR^= GPIO_Pin_4;     //PE2引脚的电平取反
    }
//    if(Delay1msCount<1)
//    {
//        Delay1msCount=0;
//          GPIOA->ODR^= GPIO_Pin_3;     //PE2引脚的电平取反
//          GPIOA->ODR^= GPIO_Pin_4;     //PE2引脚的电平取反
//    }
    if(Delay1ms>=500)
    {
//       GPIOB->ODR^= GPIO_Pin_12;     //PE2引脚的电平取反
       GPIOA->ODR^= GPIO_Pin_7;     //PE2引脚的电平取反
        Delay1ms=0;
    }
}
//void Delayus(u16 delayus)//实际1us
//{
//    u16 i;
//    while(delayus--)
//    {
//        i=1;
//        while(i--)
//        {
//            ;
//        }
//    }
//}
void Delayus(u16 delayus)//实际1us
{
    u16 i;
    while(delayus--)
    {;
    }
}
void Delay1us(u16 delayus)//实际1us
{
    u16 i;

    while(delayus--)
    {
        i=4;
        while(i--)
        {
            ;
        }
    }
}
/************************
功能:delayms
版本:V1.00   日期:11.07.15
修改:xiaodong.liu
作者:value
************************/
void Delayms(ulong delay)
{
    Delay1msCount=delay;
    while(Delay1msCount>0)
    {
        ;
    }
}

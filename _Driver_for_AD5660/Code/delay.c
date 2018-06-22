//#define DELAY_GLOBALS
#include "config.h"
uint Delay1msCount=0;
uint Delay1ms=0;
uint Delay10ms=0;
uint ItemDelay1ms=0;
int OverTime;
//#include "GlobalDef.h"
/************************
����:SysTick_Handler dell
�汾:V1.00   ����:11.10.06
�޸�:xiaodong.liu
����:value
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
//          GPIOA->ODR^= GPIO_Pin_3;     //PE2���ŵĵ�ƽȡ��
//          GPIOA->ODR^= GPIO_Pin_4;     //PE2���ŵĵ�ƽȡ��
    }
//    if(Delay1msCount<1)
//    {
//        Delay1msCount=0;
//          GPIOA->ODR^= GPIO_Pin_3;     //PE2���ŵĵ�ƽȡ��
//          GPIOA->ODR^= GPIO_Pin_4;     //PE2���ŵĵ�ƽȡ��
//    }
    if(Delay1ms>=500)
    {
 //       GPIOB->ODR^= GPIO_Pin_12;     //PE2���ŵĵ�ƽȡ��
 //       GPIOB->ODR^= GPIO_Pin_7;     //PE2���ŵĵ�ƽȡ��
        Delay1ms=0;
    }
}
void Delayus(u16 delayus)//ʵ��1us
{
    u16 i;
    while(delayus--)
    {
        i=5;
        while(i--)
        {
            ;
        }
    }
}
void Delay1us(u16 delayus)//ʵ��1us
{
    uint i;
    i=100;
    while(i--)
    {
        ;
    }
// ;
}
/************************
����:delayms
�汾:V1.00   ����:11.07.15
�޸�:xiaodong.liu
����:value
************************/
void Delayms(ulong delay)
{
    Delay1msCount=delay;
    while(Delay1msCount>0)
    {
        ;
    }
}

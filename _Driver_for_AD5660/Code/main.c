/******************** (C) COPYRIGHT 2012 WildFire Team **************************
 * �ļ���  ��main.c
 * ����    ����3.5.0�汾���Ĺ���ģ�塣
 * ʵ��ƽ̨��Ұ��STM32������
 * ��汾  ��ST3.5.0
 *
 * ����    ��wildfire team
 * ��̳    ��http://www.amobbs.com/forum-1008-1.html
 * �Ա�    ��http://firestm32.taobao.com
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
 * ��������main
 * ����  : ������
 * ����  ����
 * ���  : ��
 */
int main(void)
{

    SystemInit (); //ϵͳ��ʼ�� ���ùٷ���
    SysTick_Config(72000);//Set SysTick to 1ms   SystemCoreClock=8MHZ,72000000/1000=72000  ���ùٷ���
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

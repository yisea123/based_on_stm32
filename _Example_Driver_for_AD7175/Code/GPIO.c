#include "config.h"
//GPIO_InitTypeDef GPIO_InitStructure;
//GPIO_InitTypeDef GPIO_InitStructure;
//USART_InitTypeDef USART_InitStructure;
void GPIO_Configuration(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);   //¹¦ÄÜ¸´ÓÃIOÊ±ÖÓ
    //GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);  //¹Ø±ÕJTAGµ÷ÊÔ ½«µ÷ÊÔÒý½ÅÅäÖÃÎªÆÕÍ¨IO¶Ë¿Ú
    //GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
//    GPIO_PinRemapConfig(GPIO_Remap_PD01,ENABLE);          //Ò»¶¨ÒªÏÈ¿ªÊ±ÖÓ,ÔÙREMAP,·ñÔòÓÐÐ©½Å(JTAG),²»¶¯×÷
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  | GPIO_Pin_1  |\
                                  GPIO_Pin_2  | GPIO_Pin_3  |\
                                  GPIO_Pin_4  | GPIO_Pin_5  |\
                                  GPIO_Pin_6  | GPIO_Pin_8  |\
                                  GPIO_Pin_11 | GPIO_Pin_12 |\
                                  GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
//=======================================================================B
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1| GPIO_Pin_2|\
                                  GPIO_Pin_3| GPIO_Pin_4|\
                                  GPIO_Pin_5| GPIO_Pin_6|\
                                  GPIO_Pin_7| GPIO_Pin_8|\
                                  GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
//==============================================================================C
    // GPIOC Periph clock enable /
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    // PC output pushpull mode
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1| GPIO_Pin_2| \
                                  GPIO_Pin_3| GPIO_Pin_4| GPIO_Pin_5| \
                                  GPIO_Pin_6| GPIO_Pin_7| GPIO_Pin_8|  \
                                  GPIO_Pin_9| GPIO_Pin_10 |GPIO_Pin_12|GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode =GPIO_Mode_Out_PP;// GPIO_Mode_Out_OD;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
//   // LCD BUSY
//   // Configure PC as input GPIO_Mode_IPU
//  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_11;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;     //ÉÏÀ­
//  GPIO_Init(GPIOC, &GPIO_InitStructure);
}
/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : ÓÅÏÈ¼¶ÅäÖÃ
* Input          : None
* Output         : None
* Return         : None
* Author             : value
* Time           : 2011-08-07
*******************************************************************************/
void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    //NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x2000);
    // ÅäÖÃÓÅÏÈ¼¶·Ö×é£¬ÎÞÇÀÕ¼Ê½ÓÅÏÈ¼¶
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    /* Enable the USART1 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //ÓÅÏÈ¼¶
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    /* Enable the USART2 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //ÓÅÏÈ¼¶
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    /* Enable the USART3 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //ÓÅÏÈ¼¶
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
//===========================================================================
//rind PGA vo
//0-20mA 200 0-4V
//20-40mA 100 2-4V
//40-80mA 50 2-4V
//80-160mA 25 2-4V
float CurrentTest(uint8_t CH,uint8_t I_PGA)
{
    float Vx;
    if(CH == 1) //3V3
    {
        I_SW1;
        switch (I_PGA)
        {
        case 25:
            PGA_25_3V3;
            break;
        case 50:
            PGA_50_3V3;
            break;
        case 100:
            PGA_100_3V3;
            break;
        case 200:
            PGA_200_3V3;
            break;
        }
    }
    if(CH == 2) //3V3
    {
        I_SW2;
        switch (I_PGA)
        {
        case 25:
            PGA_25_1V8;
            break;
        case 50:
            PGA_50_1V8;
            break;
        case 100:
            PGA_100_1V8;
            break;
        case 200:
            PGA_200_1V8;
            break;
        }
    }
//  Get_ADC_Reault(0x8001,0x1320);
//  Vx = (float)((2.5*AD7175_regs[Data_Register].value)/16777216.0-1.25)*2;
    Vx = Get_ADC_Reault_ADC_average(0x8001,0x1320);
    Vx = (float)((2.5*Vx)/16777216.0-1.25)*2;
    printf("Vx=%f\r\n",Vx);
    switch (I_PGA)
    {
    case 25:
        Vx = ((Vx/25)*1000);
        break;
    case 50:
        Vx = ((Vx/50)*1000);
        break;
    case 100:
        Vx = ((Vx/100)*1000);
        break;
    case 200:
        Vx = ((Vx/200)*1000);
        break;
    }
    return Vx;
}

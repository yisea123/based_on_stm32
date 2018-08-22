/**
  *-----------------------------------------------------------------------------
  * @file    main.c
  * @author  V
  * @version V1.0
  * @date    27-NOV-2017
  * @brief   This file provides all the main functions.

  *------------------------------------------------------------------------------
  */

#include "main.h"

// BPS > 14400乱码
#define RS232_BAUDRATE		115200
#define RS485_BAUDRATE      115200

int main(void)
{
    /* System Init and NVIC Group Priority configuration -----------------------*/
    SystemInit();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    /*-------------------------- system boot ------------------------------*/
    SystemBoot();

     while (1)
    {        
        Blink(SysRuningMode, 500);
        CAN_RecAnalysis();        
    }
}

void PeriphDeviceInit(void)
{

    /* Periph device init ------------------*/
    GPIO_AFIODeInit(); // add
    EXTI_DeInit();
    Systick_Configuration();
    BlinkInit();
    USARTx_Configuration(USART1, RS232_BAUDRATE); //configurate RS232
    USARTx_Configuration(USART3, RS485_BAUDRATE); //configurate RS485
    CurrentTriggerConfigurate();
    _IO_PortInit();
    CounterConfig();
    ADC_Configuration();
    
//    DC_MOTOR_Periph(DISABLE);
    DC_MOTOR_Init();
#ifdef AC200
    DogAdopt();
#endif

}
/** 
 * @brief: system boot, self check, 
*/
void SystemBoot(void)
{
    ErrorStatus errorTemp = ERROR;

    PeriphDeviceInit();
    

    if (ERROR == SelfCheck())
    {
        while (1)
        {
            Blink(SystemSelfCheckError, 0);
            #ifdef AC200_DEBUG
                /** Error status ---------------------*/
                CAN_SendMessage.CAN_Standard_ID_No = AC200_TICK_ID;
                CAN_SendMessage.CAN_SendDataLength = 8;
                CAN_SendMessage.CAN_DATA[0] = AC200_Error;
                CAN_SendMessage.CAN_DATA[1] = 0x00;
                CAN_SendMessage.CAN_DATA[2] = 0x00;
                CAN_SendMessage.CAN_DATA[3] = 0x00;
                CAN_SendMessage.CAN_DATA[4] = 0x00;
                CAN_SendMessage.CAN_DATA[5] = 0x00;
                CAN_SendMessage.CAN_DATA[6] = 0x00;
                CAN_SendMessage.CAN_DATA[7] = 0x00;
                CAN_Send_Message(&CAN_SendMessage);
                DelayMs(1000);
                /** Error status ---------------------*/
                printf("\r\nSystem SelfCheck error\r\n");
            #endif
        }
    }
    CAN_Configurate(CAN1, CAN_Mode_Normal, CAN_SJW_1tq, CAN_BS1_9tq, CAN_BS2_8tq, 4);
        
    Blink(SysInitMode, 300);
    /** heart beat enable start ---------------------*/
    HeartBeat(ENABLE); // CAN heart beat
    /** heart beat enable end ---------------------*/
    /** send boot success ---------------------*/
    CAN_SendMessage.CAN_Standard_ID_No = AC200_TICK_ID;
    CAN_SendMessage.CAN_SendDataLength = 8;
    CAN_SendMessage.CAN_DATA[0] = AC200_Ready;
    CAN_SendMessage.CAN_DATA[1] = 0x00;
    CAN_SendMessage.CAN_DATA[2] = 0x00;
    CAN_SendMessage.CAN_DATA[3] = 0x00;
    CAN_SendMessage.CAN_DATA[4] = 0x00;
    CAN_SendMessage.CAN_DATA[5] = 0x00;
    CAN_SendMessage.CAN_DATA[6] = 0x00;
    CAN_SendMessage.CAN_DATA[7] = 0x00;
    CAN_Send_Message(&CAN_SendMessage);
    /** send boot success end ---------------------*/
    #ifdef AC200_DEBUG
        printf("\r\nSystem SelfCheck completed\r\n");
    #endif
    /** waitting master command -------------------*/
    while (errorTemp == ERROR)
    {
        #ifdef AC200_DEBUG
            printf("\r\nstuck in...\r\n");
        DelayMs(500);
        #endif
        Blink(SystemWaitting, 300);
        if (CAN_Rec_Buff[0].MasterCMD_ID == MASTER_TICK_ID)
        {
            if (AC200_Start == ((CAN_Rec_Buff[0].MasterCMD_ActionSortNumber >> 8) & 0XFF))
            {
                /** send boot success ---------------------*/
                CAN_SendMessage.CAN_Standard_ID_No = AC200_TICK_ID;
                CAN_SendMessage.CAN_SendDataLength = 8;
                CAN_SendMessage.CAN_DATA[0] = AC200_Start;
                CAN_SendMessage.CAN_DATA[1] = 0x00;
                CAN_SendMessage.CAN_DATA[2] = 0x00;
                CAN_SendMessage.CAN_DATA[3] = 0x00;
                CAN_SendMessage.CAN_DATA[4] = 0x00;
                CAN_SendMessage.CAN_DATA[5] = 0x00;
                CAN_SendMessage.CAN_DATA[6] = 0x00;
                CAN_SendMessage.CAN_DATA[7] = 0x00;
                CAN_Send_Message(&CAN_SendMessage);
                /** send boot success end ---------------------*/
                errorTemp = SUCCESS;
            }
            else
                errorTemp = ERROR;
        }
        else
            errorTemp = ERROR;
        
        memset(&CAN_Rec_Buff[0], 0, sizeof(CAN_Rec_Buff[0]));
        CAN_RecDataSort(); //sort data
            
    }
    #ifdef AC200_DEBUG
        printf("\r\n/*-------------- System Start... --------------------/\r\n");
    #endif
}
ErrorStatus SelfCheck(void)
{
     if(SUCCESS == (ADC_SelfCheck()) && (CAN_SelfCheck()))
         return SUCCESS;
     else
        return ERROR;
}

/**
 * @brief: 
 * 
 */
ErrorStatus ADC_SelfCheck(void)
{
    ErrorStatus errorTemp;
	float temp = 0;
	temp = GetChipVerf_VOL_AVE(6);
	if((temp < 1.3) && (temp > 1.1))
	{
        temp =  0;
		temp =  GetMotor_VCC_5V_Vlotage(6);
		if((temp < 5.1) && (temp > 4.9))
		{
            DC_MOTOR_Power(ENABLE);
            /** discard first sample for motor current -------------------------------*/
            GetMotorCurrentOriginal(6);
            temp = 0;
            for(uint8_t checkTimes = 0; checkTimes < 5; checkTimes++)
            {
                temp += GetMotorCurrentOriginal(6);
            }
            temp = (temp / 5);
			if((temp > 1.5) && (temp < 1.6))
				errorTemp = SUCCESS;
            DC_MOTOR_Power(DISABLE);
		}
		else
			errorTemp = ERROR;
	}
	else
		errorTemp = ERROR;
    return errorTemp;

}

/**
 * @brief:      check peripheral device function.
 *              configurate CAN as loopback mode.
 * @param:      NULL
 * @return:     return check answer, Error or Success. @refer ErrorStatus.
 */
ErrorStatus CAN_SelfCheck(void)
{
    ErrorStatus errorTemp;
    CAN_Configurate(CAN1, CAN_Mode_LoopBack, CAN_SJW_1tq, CAN_BS1_9tq, CAN_BS2_8tq, 4);  
    CAN_SendMessage.CAN_Standard_ID_No   = 0x500;
    CAN_SendMessage.CAN_SendDataLength = 8;
    CAN_SendMessage.CAN_DATA[0] = AC200_SelfCheck;
    CAN_SendMessage.CAN_DATA[1] = 0xAA;
    CAN_SendMessage.CAN_DATA[2] = 0x00;
    CAN_SendMessage.CAN_DATA[3] = 0xAA;
    CAN_SendMessage.CAN_DATA[4] = 0x00;
    CAN_SendMessage.CAN_DATA[5] = 0xAA;
    CAN_SendMessage.CAN_DATA[6] = 0x00;
    CAN_SendMessage.CAN_DATA[7] = 0xAA;
    CAN_Send_Message(&CAN_SendMessage);
    DelayMs(100);

    if(CAN_Rec_Buff[0].MasterCMD_ID == 0x500)
    {
        if (CAN_Rec_Buff[0].MasterCMD_ActionSortNumber == ((uint16_t)(AC200_SelfCheck << 8) | 0xAA))
        {
            if (CAN_Rec_Buff[0].MasterCMD_Reserved == 0X00)
            {
                if (CAN_Rec_Buff[0].MasterCMD_DataLength == 0XAA)
                {
                    if (CAN_Rec_Buff[0].MasterCMD_Param_1 == 0X00)
                    {
                        if (CAN_Rec_Buff[0].MasterCMD_Param_2 == 0XAA)
                            if (CAN_Rec_Buff[0].MasterCMD_Param_3 == 0X00)
                                if(CAN_Rec_Buff[0].MasterCMD_Param_4 == 0xAA)
                                    errorTemp = SUCCESS;
                                else
                                    errorTemp = ERROR;
                            else
                                errorTemp = ERROR;
                        else
                            errorTemp = ERROR;
                    }
                    else
                        errorTemp = ERROR;
                }
                else
                    errorTemp = ERROR;
            }
            else
                errorTemp = ERROR;
        }
        else
            errorTemp = ERROR;
    }
    else
    {
        errorTemp = ERROR;
    }
    memset(&CAN_Rec_Buff[0], 0, sizeof(CAN_Rec_Buff[0]));
    CAN_RecDataSort();  //sort data
    return errorTemp;
}

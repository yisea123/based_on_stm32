/**
  *-----------------------------------------------------------------------------
  * @file    can.h
  * @author  V
  * @version V1.0
  * @date    27-NOV-2017
  * @brief   This file contains all the functions prototypes for the can communication 
  *          driver library.
  *------------------------------------------------------------------------------
  */

#ifndef __CAN_H
#define __CAN_H

#include "stm32f10x_conf.h"
#include "usart.h"
#include "delay.h"
#include "command.h"
#include "string.h"
#include "io_operation.h"
#include "blink.h"
#include "motor_driver.h"
#include "timmer.h"

// #define CAN_RX0_INT_ENABLE // CAN BUS bps marco definition

/* Private define ------------------------------------------------------------*/
#ifdef  CAN_PRESCALER_SPEED_CONFIG
    #define CAN_BAUDRATE_1MBPS      (uint8_t)0xfA           /* 1MBps   */
    #define CAN_BAUDRATE_500KBPS    (uint8_t)0xfB           /* 500kBps */
    #define CAN_BAUDRATE_250KBPS    (uint8_t)0xfC           /* 250kBps */
    #define CAN_BAUDRATE_125KBPS    (uint8_t)0xfD           /* 125kBps */
#endif
/*----------------------------------------------------------------------------*/

#define ADDRESS_BUFF  125

/** 
 * --------------- CAN_SendMessage_TypeDef -----------------------------
     * CAN_StandardID:   CAN SEND ID value
     * CAN_ExtenID:      CAN SEND extern ID value
     * CAN_Send_ID_Mode: CAN ID type, can be set to  
         * 1. CAN_Id_Standard
         * 2. CAN_Id_Extended
     * CAN_DATA[8]:      Data need to be sent
     * CAN_SendDataLength: specified the number of data need to be sent
     * CAN_ReadyToSendFlag: Flage to show CAN Send parameter has be loaded
     *                      when loaded the data, flag need to set to ture.
    ---------------------------------------------------------------------*/

typedef struct{
    uint32_t CAN_Standard_ID_No;               
    uint8_t  CAN_DATA[8];
    uint8_t  CAN_SendDataLength;
    truthTable  CAN_ReadyToSendFlag;
}CAN_SendMessage_TypeDef;

extern CAN_SendMessage_TypeDef  CAN_SendMessage;


typedef struct{
    uint16_t slave_ID_Mode;
    uint16_t slave_ID_Type;
}CAN_ID_Mode_TypeDef;

/*---------------------- AC200 dataType ---------------------------*/
/**
 * Data frame specification: CAN Rec data[0] = Action sort number higher 8-bit
 *                          CAN Rec data[0] = Action sort number low 8 - bit
 */
typedef struct
{
    uint16_t MasterCMD_ID;
    uint16_t MasterCMD_ActionSortNumber;    //Data[0:1]
    uint8_t MasterCMD_Reserved;             //Data[2]        
    uint8_t MasterCMD_DataLength;           //Data[3]
    uint8_t MasterCMD_Param_1;              //Data[4]
    uint8_t MasterCMD_Param_2;              //Data[5]
    uint8_t MasterCMD_Param_3;              //Data[6]
    uint8_t MasterCMD_Param_4;              //Data[7]
    uint8_t emptyIndex;
} MasterCMD_SortTypeDef;
extern MasterCMD_SortTypeDef    CAN_Rec_Buff[10];
extern MasterCMD_SortTypeDef    *pCAN_Rec_Buff;


/*-------------------- function point --------------------------*/
//typedef void (*pFunc)(void, void);
//extern pFunc funcitonPoint(uint16_t x);


//uint16_t addressPoint[ADDRESS_BUFF];
/*-------------------- function point end ----------------------*/
void CAN_GPIO_Configuration(CAN_TypeDef* CANx);
void CAN_ConfigurateByMarco(CAN_TypeDef *CANx, uint8_t CAN_BPS);
void CAN_Configurate(CAN_TypeDef *CANx, uint8_t CAN_ModeSet, uint8_t CAN_SJW_TimeSet,
                                         uint8_t CAN_BS1_TimeSet, uint8_t CAN_BS2_TimeSet,
                                         uint8_t CAN_Prescaler_Set);

void CAN_PrepareSendMessage(void);
ErrorStatus CAN_Send_Message(CAN_SendMessage_TypeDef *CAN_SendData);

ErrorStatus CAN_RecAnalysis(void);
void CAN_RecDataSort(void);
void CAN_DataMerge(void);

ErrorStatus CAN_ActionAnalysis(void);

void CAN_DataCopy(CanRxMsg *CAN_Rec);
ErrorStatus AC200_System_Check(void);
void AC200_Init_Protocol(void);

void CAN_Analysis_FunctionPoint(CanRxMsg *CAN_Rec);

ErrorStatus ActionWriteIO(uint16_t _IO_Number);

ErrorStatus ActionWriteBit(uint16_t bitNumber, uint8_t bitParam);

void CAN_ActionAnalysis_MasterCMD(void);
void CAN_ActionAnalysis_Motor(void);
void CAN_ActionAnalysis_IO(void);
void CAN_ActionAnalysis_ENCO(void);
#endif

/*-------------------------------------- END LINE -----------------------------------------*/






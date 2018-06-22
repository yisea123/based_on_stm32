/**
  *-----------------------------------------------------------------------------
  * @file    command.h
  * @author  V
  * @version V1.0
  * @date    27-NOV-2017
  * @brief   This file contains all the functions prototypes for 
  *          command define and driver library.
  *------------------------------------------------------------------------------
  */

#ifndef __COMMAND_H
#define __COMMAND_H

#include <stdint.h>
#include "system_stm32f10x.h"
#include "math.h"

/*------------------------ Board support choose --------------------------*/
// #define WARESHARE_BOARD     // wareshare board support
// #define AC200                   // AC200 support
#define AC200_DEBUG         // AC200 debug support

#define TRUTH_TABLE         // for compatibility reasons
/*-----------------------------------------------------------------------*/
#ifdef TRUTH_TABLE
    typedef enum {
        NG = 0,
        OK  = !NG
    } truthTable;
#endif
/*-------------------------- GPIO define part ---------------------------*/
/** PORT define --------------------------*/
#define         PORTA       GPIOA
#define         PORTB       GPIOB
#define         PORTC       GPIOC
#define         PORTD       GPIOD


/*--------------------- can comunication part ---------------------------*/
/*-------------------- Divide CAN ID part ----------------*/
#define AC200_NODE_ID               (uint8_t)0x7D
#define AC200_FUNCTION_CODE         (uint8_t)0x50
/*-------------------- Specifies CAN ID ------------------*/

#define AC200_EMG_ID        (((uint16_t)0x80 << 4) + AC200_NODE_ID)
#define AC200_TICK_ID       (((uint16_t)0x50 << 4) + AC200_NODE_ID)
#define MASTER_TICK_ID      (((uint16_t)0x50 << 4) + 0xAA)

#define RESERVED_NUMBER     0xff
/*-------------------- Emergency Events ------------------*/
typedef enum {
    EMG_Full_Stop  = 0x00,
    EMG_Motor_Stop = 0x01

} AC200_EMG_TypeEnum;

/*------------------------------- CAN ID part ---------------------------------*/
typedef enum {
    MasterCMD         = (((uint16_t)AC200_FUNCTION_CODE << 4) + 0x10),
    MasterCMD_MOTOR   = (((uint16_t)AC200_FUNCTION_CODE << 4) + 0x20),
    MasterCMD_IO      = (((uint16_t)AC200_FUNCTION_CODE << 4) + 0x30),
    MasterCMD_Search  = (((uint16_t)AC200_FUNCTION_CODE << 4) + 0x40),
    MasterCMD_Data    = (((uint16_t)AC200_FUNCTION_CODE << 4) + 0x50),
    MasterCMD_HoldOn  = (((uint16_t)AC200_FUNCTION_CODE << 4) + 0x60),
    MasterCMD_Recover = (((uint16_t)AC200_FUNCTION_CODE << 4) + 0x70),
	MasterCMD_ENCO	  = (((uint16_t)AC200_FUNCTION_CODE << 4) + 0x80)
    
}AC200_CMD_TypeEnum;

extern AC200_CMD_TypeEnum   CAN_MasterCMD_ID;

/*---------------------------- AC200 System status ---------------------------*/
typedef enum{
    AC200_Ready         = 0x7F,
    AC200_Start         = 0x01,
    AC200_Stop          = 0x04,
    AC200_HeartBeat     = 0x05,
    AC200_Error         = 0x06,
    AC200_Running       = 0x07,
    AC200_Reboot        = 0xAA
}AC200_Sys_StatusTypeEnum;
extern AC200_Sys_StatusTypeEnum             AC200_Sys_Status;

/*---------------------------- AC200 Action status ---------------------------*/
typedef enum {
    AC200_ActionInit             = 0x00,
    AC200_ActionDone             = 0x01,
    AC200_ActionRuning           = 0x02,
    AC200_ActionPause            = 0x03,
    AC200_ActionError            = 0x04,
    AC200_ActionCanNotDo         = 0x05,
    AC200_ActionAnalysisError    = 0x06,
    AC200_ActionAnalysisReserved = 0x07,
    AC200_SelfCheck              = 0x0A,
    AC200_Busy                   = 0x0F


}AC200_Action_StatusTypeEnum;
extern AC200_Action_StatusTypeEnum          AC200_Action_Status;

/*---------------------- AC200 Action number enum ---------------------------*/
typedef enum{

    /*------------- motor part ------------------------------------*/
    AC200_Motor_Part_Start          = 0x2000,
    AC200_MotorBreak                = 0x2001,
    AC200_MotorStop                 = 0x2002,
    AC200_MotorShutdown             = 0x2003,
    AC200_MotorReady                = 0x2004,
    AC200_MotorStart                = 0x2005,
    AC200_MotorRun                  = 0x2006,
    AC200_MotorSpeedUp              = 0x2007,
    AC200_MotorReduceSpeed          = 0x2008,
    AC200_MotorChangeDir            = 0x2009,
    AC200_MotorStartSpeedSet        = 0x200A,
    AC200_MotorMaxSpeedSet          = 0x200B,
    AC200_MotorMinSpeedSet          = 0x200C,
    AC200_MotorAcceSet              = 0x200D,       //accelerate set
    AC200_MotorMax_PWM_DutyCycleSet = 0x200E,
    AC200_MotorMin_PWM_DutyCycleSet = 0x200F,
    AC200_Motor_PWM_FreqSet         = 0x2010,
    AC200_Motor_SelfCheck           = 0x2011,
    AC200_Motor_SpeedCheck          = 0x2012,
    AC200_Motor_Part_End            = 0x2FFF,

    /*------------- motor encoder part ------------------------------*/
    AC200_ENCO_Part_Start           = 0x3000,
    AC200_ENCO_ON                   = 0x3001,
    AC200_ENCO_OFF                  = 0x3002,
    AC200_ENCO_SelfCheck            = 0x3003,
    AC200_ENCO_DataRead             = 0x3004,
    AC200_ENCO_Part_End             = 0x3FFF,

    /*------------------ IO Read part ------------------------------*/
    AC200_Read_IO_Part_Start        = 0x4000,
    AC200_Read_IO_1                 = 0x4001,
    AC200_Read_IO_2                 = 0x4002,
    AC200_Read_IO_3                 = 0x4003,
    AC200_Read_IO_4                 = 0x4004,
    AC200_Read_IO_5                 = 0x4005,
    AC200_Read_IO_6                 = 0x4006,
    AC200_Read_IO_7                 = 0x4007,
    AC200_Read_IO_8                 = 0x4008,
    AC200_Read_IO_9                 = 0x4009,
    AC200_Read_IO_10                = 0x400A,
    AC200_Read_IO_ByLabel           = 0x45FF,
    AC200_Read_IO_Part_End          = 0x46FF,

    /*------------------ IO set part ------------------------------*/
    AC200_Write_IO_Part_Start       = 0x4700,
    AC200_Write_IO_1                = 0x4701,
    AC200_Write_IO_2                = 0x4702,
    AC200_Write_IO_3                = 0x4703,
    AC200_Write_IO_4                = 0x4704,
    AC200_Write_IO_5                = 0x4705,
    AC200_Write_IO_6                = 0x4706,
    AC200_Write_IO_7                = 0x4707,
    AC200_Write_IO_8                = 0x4708,
    AC200_Write_IO_9                = 0x4709,
    AC200_Write_IO_10               = 0x470A,
    AC200_Write_IO_ByLabel          = 0x4EFF,
    AC200_Write_IO_Part_End         = 0x4FFF
    
}AC_200_ActionSortNumberTypeEnum;
extern AC_200_ActionSortNumberTypeEnum      AC200_ActionSortNumber;

extern uint8_t AC_200_StatusNumber;
#endif

/*-------------------------------------- END LINE -----------------------------------------*/



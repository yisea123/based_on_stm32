/**
  *-------------------------------------------------------------------------------------------------
  * @file    command.c
  * @author  V
  * @version V1.0
  * @date    27-NOV-2017
  * @brief   This file provides all the command define and driver functions
  *-----------------------------------------------------------------------------------------------------
  */
#include "command.h"



AC200_CMD_TypeEnum                      CAN_MasterCMD_ID;
AC200_Sys_StatusTypeEnum                AC200_Sys_Status;
AC200_Action_StatusTypeEnum             AC200_Action_Status;
AC_200_ActionSortNumberTypeEnum         AC200_ActionSortNumber;

uint8_t AC_200_StatusNumber = AC200_HeartBeat;

/*-------------------------------------- END LINE -----------------------------------------*/




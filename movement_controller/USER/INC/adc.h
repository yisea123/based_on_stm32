/**
  *-----------------------------------------------------------------------------
  * @file    adc.h
  * @author  V
  * @version V1.0
  * @date    27-NOV-2017
  * @brief   This file contains all the functions prototypes for the ADC library.
  *------------------------------------------------------------------------------
  * ADC1_Channel1 -> PA1 -> VCC_5V; 
  * ADC1_Channel2 -> PA2 -> Current motor current
  */
#ifndef __ADC_H
#define __ADC_H

#include "stm32f10x.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"
#include "command.h"
#include "usart.h"
#include "motor_driver.h"

#define   ADC_5V_Voltage          GPIO_Pin_1            //PA1 (ADC Channel1)
#define   ADC_MOTOR_CURRENT       GPIO_Pin_2            //PA2 (ADC Channel2)

void ADC_GPIO_Config(void);
void ADC_Configuration(void);

uint16_t  Get_ADC_Value(uint8_t ADC_Channel); 
uint16_t  Get_Adc_Average(uint8_t ADC_Channel,uint8_t times);
extern float GetChipInsideTempAve(uint8_t times);
extern float GetChipVerf_VOL_AVE(uint8_t times);
extern float GetMotorCurrentOriginal(uint8_t times);
extern float GetMotorCurrentAVE(uint8_t times);
extern float GetMotor_VCC_5V_Vlotage(uint8_t times);


#endif

/*--------------------------------------- END LINE ------------------------------------*/


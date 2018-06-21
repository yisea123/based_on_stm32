/************************************************
文件：config.h
用途：系统配置函数
************************************************/
#define uint  unsigned int
#define uchar  unsigned char
#define ulong  unsigned long

#ifndef __config_H__
#define __config_H__
//#define u16  unsigned int
//#define u8  unsigned char
//#define u32  unsigned long
#include "stm32f10x.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include  <math.h>
#include <stdarg.h>
#include "stdint.h"
#include "sys.h"
#include "uart.h"
#include "GPIO.h"
#include "delay.h"
//#include "TFT.h"
#include "AD7175.h"
#include "AD7175_regs.h"
#include "SPI_By_IO.h"
#include "Communication.h"
#include "I2C_1.h"
#include "AD5660.h"
#include "AD9833.h"
#include "24cxx.h"
#include "IIC.h"
#include "tmp102.h"
#include "SPI_DUT.h"
#include "DUT_ADC.h"
//#include "TIM3.h"

extern GPIO_InitTypeDef GPIO_InitStructure;
extern USART_InitTypeDef USART_InitStructure;
#endif

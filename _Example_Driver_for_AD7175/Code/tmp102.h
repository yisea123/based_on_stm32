#ifndef _TMP102_H
#define _TMP102_H
//#include"stm32f10x.h"
//#include"iic.h"
//#include "systickdelay.h"
/********************************************************
 * TMP102所有操作函数
 ********************************************************/
/******* IIC的IO口初始化 *************************/
void TMP102_Init(void);
/******* 指定地址读取两个字节 *************************/
uint32_t TMP102_ReadTwoByte(uint16_t PointerAddr);
/******* 指定地址写入两个字节 *************************/
void TMP102_WriteTwoByte(uint16_t PointerAddr,uint16_t Data1Write,uint16_t Data2Write);
/******* 求温度平均值 *************************/
float  temp_average(uint8_t times);
extern uint32_t Complement(uint32_t data);
#endif

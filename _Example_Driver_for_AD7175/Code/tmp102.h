#ifndef _TMP102_H
#define _TMP102_H
//#include"stm32f10x.h"
//#include"iic.h"
//#include "systickdelay.h"
/********************************************************
 * TMP102���в�������
 ********************************************************/
/******* IIC��IO�ڳ�ʼ�� *************************/
void TMP102_Init(void);
/******* ָ����ַ��ȡ�����ֽ� *************************/
uint32_t TMP102_ReadTwoByte(uint16_t PointerAddr);
/******* ָ����ַд�������ֽ� *************************/
void TMP102_WriteTwoByte(uint16_t PointerAddr,uint16_t Data1Write,uint16_t Data2Write);
/******* ���¶�ƽ��ֵ *************************/
float  temp_average(uint8_t times);
extern uint32_t Complement(uint32_t data);
#endif

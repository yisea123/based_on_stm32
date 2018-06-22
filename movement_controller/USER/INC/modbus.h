/**
 * using crc16 to check data
 */
#ifndef __PROTOCOL_H
#define __PROTOCOL_H

#include "stm32f10x.h"
#include "usart.h"
#include "crc_check.h"
#include "command.h"


#define M_BUFF_SIZE			100

#define M_MAX_FRAME_LENGTH	261		//最大帧长度为261字节 地址+功能+帧号+数据长度+数据+CRC(CRC16有2字节)
#define M_MIN_FRAME_LENGTH	5		//最小帧长度为5字节 地址+功能+帧号+数据长度+CRC8/XOR/SUM

typedef truthTable  Modbus_status;

typedef struct
{
	uint8_t M_usartRxBuff[M_BUFF_SIZE];	
	uint8_t M_usartTxBuff[M_BUFF_SIZE];
	uint8_t M_usartRecLength;
	uint8_t M_usartSentLength;

	Modbus_status frameStatus;		// frame status
	Modbus_status frameFormat;		// frame format status
	Modbus_status crc_checkStatus;	// frame crc check answer
	Modbus_status framelength;		// frame total length

}ModBusTypedef;

typedef struct
{
	uint8_t frameAddress;			//address
	uint8_t frameFunction;			//function code
	uint8_t frameDatalength;		//data length
	uint16_t CRC16_Value;			// crc value
	
}Modbus_FrameTypedef;


extern Modbus_FrameTypedef 		Modbus_Frame;
extern ModBusTypedef 			ModBus;

extern Modbus_status analysisFrame(Modbus_FrameTypedef *frame);
extern void packFrame(void);
extern void sendFrame(void);


#endif














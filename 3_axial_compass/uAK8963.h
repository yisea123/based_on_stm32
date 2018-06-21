

#ifndef _BSP_AK8963_H
#define _BSP_AK8963_H

//#define ulong           unsigned long
//#define ushort 			unsigned short
#define uint8_t 			unsigned char
#define uint16_t 			unsigned int
#define int16_t 			int

#define AK8963_SLAVE_ADDRESS    0x18		/* I2C从机地址 */
#define AK8963_Device_ID            ((unsigned char)0x48)

// Read-only Reg
#define AK8963_WIA                  ((unsigned char)0x00)
#define AK8963_INFO                 ((unsigned char)0x01)
#define AK8963_ST1                  ((unsigned char)0x02)
#define AK8963_HXL                  ((unsigned char)0x03)
#define AK8963_HXH                  ((unsigned char)0x04)
#define AK8963_HYL                  ((unsigned char)0x05)
#define AK8963_HYH                  ((unsigned char)0x06)
#define AK8963_HZL                  ((unsigned char)0x07)
#define AK8963_HZH                  ((unsigned char)0x08)
#define AK8963_ST2                  ((unsigned char)0x09)
// Write/Read Reg
#define AK8963_CNTL1                ((unsigned char)0x0A)// Continuous measurement mode 2
#define AK8963_CNTL2                ((unsigned char)0x0B)// Reset Device
#define AK8963_ASTC                 ((unsigned char)0x0C)
#define AK8963_TS1                  ((unsigned char)0x0D)
#define AK8963_TS2                  ((unsigned char)0x0E)
#define AK8963_I2CDIS               ((unsigned char)0x0F)
// Read-only Reg ( ROM )
#define AK8963_ASAX                 ((unsigned char)0x10)
#define AK8963_ASAY                 ((unsigned char)0x11)
#define AK8963_ASAZ                 ((unsigned char)0x12)
//****************************************
// 定义AK8963内部地址
//****************************************
#define DATA_OUT_X		0x03

typedef struct
{
	int16_t X;
	int16_t Y;
	int16_t Z;

	int16_t X_Min;
	int16_t Y_Min;
	int16_t Z_Min;

	int16_t X_Max;
	int16_t Y_Max;
	int16_t Z_Max;	

	uint8_t	Status;
	
	uint8_t CfgRegA;
	uint8_t CfgRegB;
	uint8_t CfgRegC;
	uint8_t ModeReg;
	
	uint8_t IDReg[3+1];	
}AK8963_T;

extern AK8963_T g_tMag;

void bsp_InitAK8963(void);

void AK8963_ReadData(void);
void AK8963_WriteByte(uint8_t _ucRegAddr, uint8_t _ucRegData);
uint8_t AK8963_ReadByte(uint8_t _ucRegAddr);
void GetAK8963Mag(void);
uint8_t Self_test(void);

#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/


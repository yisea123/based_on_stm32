
#ifndef __AD7175_H__
#define __AD7175_H__

/***************************** Include Files *********************************/
#include <stdint.h>
#include "AD7175_regs.h"

/******************* AD7175 Constants ****************************************/
#define AD7175_CRC_POLYNOMIAL 0x07 // x^8 + x^2 + x +1 (MSB first)
#define AD7175_CRC_CHECK_CODE 0xF3 

/************************ Functions Declarations *****************************/
void AD7175_Reset(void);
int32_t AD7175_Setup(void);
int32_t AD7175_ReadRegister(st_reg* pReg);
int32_t AD7175_WriteRegister(st_reg reg);
uint8_t AD7175_ComputeCRC(uint8_t* pBuf, uint8_t bufSize);
int32_t AD7175_WaitForReady(uint32_t timeout);
int32_t AD7175_ReadData(int32_t pData);

//extern void Get_ADC_Reault(u16 CH_num,u16 setup_cfg);




#define BIPOLAR_POLARITY                0x01
#define UNIPOLARITY_POLARITY            0x02
void AD7175_SetChannels(void);
void PolaritySet(uint8_t polarity);
#endif // __AD7175_H__

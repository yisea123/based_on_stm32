
#include "AD7175.h"

/************************ Local variables and types ***************************/
struct AD7175_state
{
    uint8_t useCRC;
}AD7175_st;

/**************************************************************************//**
* @brief Initializes the AD7175 
* @return Returns 0 for success or negative error code.
******************************************************************************/
void AD7175_Init(void)
{
    uint8_t Flg_Ready;
    
    SPI_CS_0();             //??ADC
    AD7175_Reset();             //ADC??
    SPI_CLK_Delay();
    AD7175_ReadRegister(&AD7175_regs[ID_st_reg]);       //??ADC_ID
    printf("ADC7175_ID=%04X\r\n",AD7175_regs[ID_st_reg].value);
    Flg_Ready = AD7175_Setup();         //??ADC
    if(Flg_Ready < 0)
    {
        printf("Init_AD7175_Fail");
    }
    SPI_CS_1();
}

/***************************************************************************//** 
* @brief Resets the device. 
*******************************************************************************/ 
void AD7175_Reset(void)
{

 	uint8_t wrBuf[10] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; 
	SPI_Write(1, wrBuf, 10); 		//send b byte 0xff to reset chip AD7175.
}

/**************************************************************************//**
* @brief Initializes the AD7175 
* @return Returns 0 for success or negative error code.
******************************************************************************/
int32_t AD7175_Setup(void)														//按照头文件中数组中定义初始化AD7175
{
    int32_t ret;
	
    /* Initialize ADC mode register */
		ret = AD7175_WriteRegister(AD7175_regs[ADC_Mode_Register]);
    if(ret < 0)	return ret;
    
    /* Initialize Interface mode register */		
    ret = AD7175_WriteRegister(AD7175_regs[Interface_Mode_Register]);
    if(ret < 0)	return ret;
    AD7175_st.useCRC = 0;						//是否启动CRC校验
    
    /* Initialize GPIO configuration register */
		ret = AD7175_WriteRegister(AD7175_regs[GPIOCON]);				
    if(ret < 0)	return ret;   
    
    /* Initialize Channel Map registers */
    ret = AD7175_WriteRegister(AD7175_regs[CH_Map_1]);					
    if(ret < 0) return ret;   
    ret = AD7175_WriteRegister(AD7175_regs[CH_Map_2]);
    if(ret < 0)	return ret;   
    ret = AD7175_WriteRegister(AD7175_regs[CH_Map_3]);
    if(ret < 0)	return ret;    
    ret = AD7175_WriteRegister(AD7175_regs[CH_Map_4]);
    if(ret < 0)	return ret;
    
    /* Initialize Setup Configuration registers */
    ret = AD7175_WriteRegister(AD7175_regs[Setup_Config_1]);			
    if(ret < 0)	return ret;
    ret = AD7175_WriteRegister(AD7175_regs[Setup_Config_2]);
    if(ret < 0)	return ret;
    ret = AD7175_WriteRegister(AD7175_regs[Setup_Config_3]);
    if(ret < 0)	return ret;
    ret = AD7175_WriteRegister(AD7175_regs[Setup_Config_4]);
    if(ret < 0)	return ret;

    /* Initialize Filter Configuration registers */
    ret = AD7175_WriteRegister(AD7175_regs[Filter_Config_1]);				//滤波寄存器操作
    if(ret < 0)	return ret;
    ret = AD7175_WriteRegister(AD7175_regs[Filter_Config_2]);
    if(ret < 0)	return ret;
    ret = AD7175_WriteRegister(AD7175_regs[Filter_Config_3]);
    if(ret < 0)	return ret;
    ret = AD7175_WriteRegister(AD7175_regs[Filter_Config_4]);
    if(ret < 0)	return ret;

    return ret;
}

/**************************************************************************//**
* @brief Reads the value of the specified register
* @param pReg - Pointer to the register structure holding info about the 
*               register to be read. The read value is stored inside the 
*               register structure.
* @return Returns 0 for success or negative error code.
******************************************************************************/
int32_t AD7175_ReadRegister(st_reg* pReg)
{
    int32_t ret       = 0;
    uint8_t buffer[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    uint8_t i         = 0;
    uint8_t crc       = 0;

    /* Build the Command word */
    buffer[0] = COMM_REG_WEN | COMM_REG_RD | pReg->addr;				    //buffer[0]	为Command 字节	，写操作，操作对象为指定的寄存器			
		
    /* Read data from the device */
    ret = SPI_Read(AD7175_SLAVE_ID, 										/*	子机编号		*/
                   buffer, 																			/*	要保存数据的缓冲区地址	*/
                   (AD7175_st.useCRC ? pReg->size + 1 : pReg->size) + 1);	/*	如果启用CRC校验，则字节数量+2，否则+1	*/
    if(ret < 0)	return ret;

    /* Check the CRC */
    if(AD7175_st.useCRC)													/*	如果启用CRC校验则计算CRC结果，并比对	*/
    { 	crc = AD7175_ComputeCRC(&buffer[1], pReg->size + 1);
        if(crc != AD7175_CRC_CHECK_CODE)	return -1;
    }

    /* Build the result */										/*	构造结果数据	*/
    pReg->value = 0;																				
    for(i = 1; i < pReg->size + 1; i++)							/*	根据要返回的数据字节数，构建结果	*/
    {		pReg->value <<= 8;													
        pReg->value += buffer[i];								/*	接收到的数据中，buffer[1]	为高字节，	buffer[3]	为低字节*/
    }
    return ret;
}


/**************************************************************************//**
* @brief Writes the value of the specified register
* @param reg - Register structure holding info about the register to be written
* @return Returns 0 for success or negative error code.
******************************************************************************/
int32_t AD7175_WriteRegister(st_reg reg) 			//wirte data to AD7175 register, address reg
{													//寄存器由参数 st_reg 传入，为一个数据结构，包含{Addr,Value,Byte_Num}
    int32_t ret      = 0;
    int32_t regValue = 0;
    uint8_t wrBuf[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    uint8_t i        = 0;
    uint8_t crc      = 0;
    
    /* Build the Command word */
		/*COMMS寄存器设定：bit7=0_通讯时必须为低电平;	bit6=0_写操作;	bit5_bit0 =	寄存器编号地址*/
    wrBuf[0] = COMM_REG_WEN | COMM_REG_WR | reg.addr;			//wrBuf[0]	保存命令字节
		
    /* Fill the write buffer */
    regValue = reg.value;									//把寄存器数组中，寄存器的初始值写入
    for(i = 0; i < reg.size; i++)							//根据该寄存器数据长度，循环写入数据
    {		wrBuf[reg.size - i] = regValue & 0xFF;			//	wrBuf[1]为最高字节		wrBuf[3] 为最低字节		
        regValue >>= 8;
    }

    /* Compute the CRC */
    if(AD7175_st.useCRC)									//如果使能了CRC校验，则把CRC的校验值取反保存到 wrBuf[4] 中
    {		crc = AD7175_ComputeCRC(wrBuf, reg.size+1);
        wrBuf[reg.size + 1] = ~crc;
    }
		
    /* Write data to the device */							//发送数据给器件
    ret = SPI_Write(AD7175_SLAVE_ID,						/*	子机地址编号	*/
                    wrBuf,																							/*	数据缓冲数组	*/
                    AD7175_st.useCRC ? reg.size + 2 : reg.size + 1);	/*	如果使能了CRC校验，则发送数据长度+2个字节，否则+1	*/
    return ret;
}


/**************************************************************************//**
* @brief Computes the CRC for a data buffer
* @param pBuf - Data buffer
* @param bufSize - Data buffer size in bytes
* @return Returns the computed CRC
******************************************************************************/
uint8_t AD7175_ComputeCRC(uint8_t* pBuf, uint8_t bufSize)						//计算CRC校验码
{
    uint8_t i = 0;
    uint8_t crc = 0xFF;

	while(bufSize--)
	{
		crc ^= *pBuf++;
		for(i = 0; i < 8; i++)
		{
			if(crc & 0x80)
				crc = (crc << 1) ^ AD7175_CRC_POLYNOMIAL;
            else
				crc <<= 1;
		}
	}
	return crc;
}


/**************************************************************************//**
* @brief Waits until a new conversion result is available
* @param timeout - Count representing the number of polls to be done until the
*                  function returns if no new data is available. 
* @return Returns 0 for success or negative error code.
******************************************************************************/
int32_t AD7175_WaitForReady(uint32_t timeout)
{
    int32_t ret;
    int8_t ready = 0;

    while(!ready && --timeout)
    {		/* Read the value of the Status Register */
        ret = AD7175_ReadRegister(&AD7175_regs[Status_Register]);
        if(ret < 0)	return ret;

        /* Check the RDY bit in the Status Register */
        ready = (AD7175_regs[Status_Register].value & STATUS_REG_RDY) != 0;
    }
    return timeout ? 0 : -1; 
}

/**************************************************************************//**
* @brief Reads the conversion result from the device.
* @param pData - Pointer to store the read data.
* @return Returns 0 for success or negative error code.
******************************************************************************/
int32_t AD7175_ReadData(int32_t pData)
{
    int32_t ret;

    /* Read the value of the Status Register */
    /*	读取结果寄存器	*/
    /*	读取完成，数据保存在	结构体数组对应的元素中，保存的内容包含{addr,Value,byte_num}		*/
    ret = AD7175_ReadRegister(&AD7175_regs[Data_Register]);

    /* Get the read result */
    /*	从结构体数组元素中，读取对应的结果值	*/
    pData = AD7175_regs[Data_Register].value;

    return ret;
}

/***************************************************************************//**
 * @brief Set channels whitch is used

 *
 * @return
*******************************************************************************/
void AD7175_SetChannels(void)
{
//    uint32_t oldRegValue, newRegValue;

//    AD7175_ReadRegister(&AD7175_regs[CH_Map_1]);
//    oldRegValue = AD7175_regs[CH_Map_1].value;
////    newRegValue = ((oldRegValue | CH_MAP_REG_SETUP(1)) | CH_MAP_REG_AINPOS(1));
//    newRegValue = oldRegValue | 0x8001;
//    AD7175_regs[CH_Map_1].value = newRegValue;
//    AD7175_WriteRegister(AD7175_regs[CH_Map_1]);
//    
//    AD7175_ReadRegister(&AD7175_regs[CH_Map_2]);
//    oldRegValue = AD7175_regs[CH_Map_2].value;
////    newRegValue = ((oldRegValue | CH_MAP_REG_SETUP(1)) | CH_MAP_REG_AINPOS(1));
//    newRegValue = oldRegValue | 0x8001;
//    AD7175_regs[CH_Map_2].value = newRegValue;
//    AD7175_WriteRegister(AD7175_regs[CH_Map_2]);
    
    
    unsigned long channelValue = CH_MAP_REG_AINPOS(1) | CH_MAP_REG_AINNEG(2);
    AD7175_WriteRegister(AD7175_regs[CH_Map_1]);
    AD7175_WriteRegister(AD7175_regs[CH_Map_2]);
    channelValue = AD7175_regs[CH_Map_3].value & CH_MAP_REG_CHEN;
    AD7175_WriteRegister(AD7175_regs[CH_Map_3]);
    AD7175_WriteRegister(AD7175_regs[CH_Map_4]);
}

void PolaritySet(uint8_t polarity)
{
    
    if(polarity == BIPOLAR_POLARITY)
    {
        AD7175_regs[Filter_Config_1].value = AD7175_regs[Filter_Config_1].value | SETUP_CONF_REG_BI_UNIPOLAR;

        unsigned long oldRegValue = 0x0;
        unsigned long newRegValue = 0x0;

        oldRegValue = AD7175_ReadRegister(&AD7175_regs[Filter_Config_1]);
        oldRegValue &= ~oldRegValue;
        newRegValue = oldRegValue | AD7175_regs[Filter_Config_1].value;
        AD7175_WriteRegister(AD7175_regs[Filter_Config_1]);
    }
    else if(polarity == UNIPOLARITY_POLARITY)
    {
        AD7175_regs[Filter_Config_1].value = AD7175_regs[Filter_Config_1].value | SETUP_CONF_REG_UNI_POLAR;

        unsigned long oldRegValue = 0x0;
        unsigned long newRegValue = 0x0;

        oldRegValue = AD7175_ReadRegister(&AD7175_regs[Filter_Config_1]);
        oldRegValue &= ~oldRegValue;
        newRegValue = oldRegValue | AD7175_regs[Filter_Config_1].value;
        AD7175_WriteRegister(AD7175_regs[Filter_Config_1]);
    }

}

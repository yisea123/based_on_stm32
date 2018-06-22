#define AD7175_INIT
#include "config.h"
/***************************** Include Files *********************************/
//#include "Communication.h"
//#include "AD7175.h"
//#include "usart.h"
//#include "AD7175_regs.h"
/************************ Local variables and types ***************************/
struct AD7175_state
{
    uint8_t useCRC;
} AD7175_st;
/***************************************************************************//**
* @brief Resets the device.
*******************************************************************************/
void AD7175_Reset(void)
{
    uint8_t wrBuf[9] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    SPI_Write(1, wrBuf, 9);         //����8���ֽ�0xff�Ը�λAD7175
}
/**************************************************************************//**
* @brief Initializes the AD7175
* @return Returns 0 for success or negative error code.
******************************************************************************/
int32_t AD7175_Setup(void)                                                      //����ͷ�ļ��������ж����ʼ��AD7175
{
    int32_t ret;
    /* Initialize ADC mode register */
    ret = AD7175_WriteRegister(AD7175_regs[ADC_Mode_Register]);
    if(ret < 0)
    {
        return ret;
    }
    /* Initialize Interface mode register */
    ret = AD7175_WriteRegister(AD7175_regs[Interface_Mode_Register]);
    if(ret < 0)
    {
        return ret;
    }
    AD7175_st.useCRC = 0;                       //�Ƿ�����CRCУ��
    /* Initialize GPIO configuration register */
    ret = AD7175_WriteRegister(AD7175_regs[GPIOCON]);
    if(ret < 0)
    {
        return ret;
    }
    /* Initialize Channel Map registers */
    printf("Map1_dat=%04X\r\n",AD7175_regs[CH_Map_1].value);
    ret = AD7175_WriteRegister(AD7175_regs[CH_Map_1]);
    if(ret < 0)
    {
        return ret;
    }
    ret = AD7175_WriteRegister(AD7175_regs[CH_Map_2]);
    if(ret < 0)
    {
        return ret;
    }
    ret = AD7175_WriteRegister(AD7175_regs[CH_Map_3]);
    if(ret < 0)
    {
        return ret;
    }
    ret = AD7175_WriteRegister(AD7175_regs[CH_Map_4]);
    if(ret < 0)
    {
        return ret;
    }
    /* Initialize Setup Configuration registers */
    ret = AD7175_WriteRegister(AD7175_regs[Setup_Config_1]);
    if(ret < 0)
    {
        return ret;
    }
    ret = AD7175_WriteRegister(AD7175_regs[Setup_Config_2]);
    if(ret < 0)
    {
        return ret;
    }
    ret = AD7175_WriteRegister(AD7175_regs[Setup_Config_3]);
    if(ret < 0)
    {
        return ret;
    }
    ret = AD7175_WriteRegister(AD7175_regs[Setup_Config_4]);
    if(ret < 0)
    {
        return ret;
    }
    /* Initialize Filter Configuration registers */
    ret = AD7175_WriteRegister(AD7175_regs[Filter_Config_1]);               //�˲��Ĵ�������
    if(ret < 0)
    {
        return ret;
    }
    ret = AD7175_WriteRegister(AD7175_regs[Filter_Config_2]);
    if(ret < 0)
    {
        return ret;
    }
    ret = AD7175_WriteRegister(AD7175_regs[Filter_Config_3]);
    if(ret < 0)
    {
        return ret;
    }
    ret = AD7175_WriteRegister(AD7175_regs[Filter_Config_4]);
    if(ret < 0)
    {
        return ret;
    }
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
    buffer[0] = COMM_REG_WEN | COMM_REG_RD | pReg->addr;                //buffer[0] ΪCommand �ֽ�  ��д��������������Ϊָ���ļĴ���
    /* Read data from the device */
    ret = SPI_Read(AD7175_SLAVE_ID,                                                         /*  �ӻ����        */
                   buffer,                                                                          /*  Ҫ�������ݵĻ�������ַ  */
                   (AD7175_st.useCRC ? pReg->size + 1 : pReg->size) + 1);           /*  �������CRCУ�飬���ֽ�����+2������+1   */
    if(ret < 0)
    {
        return ret;
    }
    /* Check the CRC */
    if(AD7175_st.useCRC)                                                                            /*  �������CRCУ�������CRC��������ȶ�    */
    {
        crc = AD7175_ComputeCRC(&buffer[1], pReg->size + 1);
        if(crc != AD7175_CRC_CHECK_CODE)
        {
            return -1;
        }
    }
    /* Build the result */                                                  /*  ����������    */
    pReg->value = 0;
    for(i = 1; i < pReg->size + 1; i++)                         /*  ����Ҫ���ص������ֽ������������    */
    {
        pReg->value <<= 8;
        pReg->value += buffer[i];                                       /*  ���յ��������У�buffer[1]   Ϊ���ֽڣ�  buffer[3]   Ϊ���ֽ�*/
    }
    return ret;
}
/**************************************************************************//**
* @brief Writes the value of the specified register
* @param reg - Register structure holding info about the register to be written
* @return Returns 0 for success or negative error code.
******************************************************************************/
int32_t AD7175_WriteRegister(st_reg reg)                                //��AD7175ָ���ļĴ�����д������
{
    //�Ĵ����ɲ��� st_reg ���룬Ϊһ�����ݽṹ������{Addr,Value,Byte_Num}
    int32_t ret      = 0;
    int32_t regValue = 0;
    uint8_t wrBuf[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    uint8_t i        = 0;
    uint8_t crc      = 0;
    /* Build the Command word */
    /*COMMS�Ĵ����趨��bit7=0_ͨѶʱ����Ϊ�͵�ƽ;   bit6=0_д����;  bit5_bit0 = �Ĵ�����ŵ�ַ*/
    wrBuf[0] = COMM_REG_WEN | COMM_REG_WR | reg.addr;           //wrBuf[0]  ���������ֽ�
    /* Fill the write buffer */
    regValue = reg.value;                                                                   //�ѼĴ��������У��Ĵ����ĳ�ʼֵд��
    for(i = 0; i < reg.size; i++)                                                   //���ݸüĴ������ݳ��ȣ�ѭ��д������
    {
        wrBuf[reg.size - i] = regValue & 0xFF;                      //  wrBuf[1]Ϊ����ֽ�      wrBuf[3] Ϊ����ֽ�
        regValue >>= 8;
    }
    /* Compute the CRC */
    if(AD7175_st.useCRC)                                                                    //���ʹ����CRCУ�飬���CRC��У��ֵȡ�����浽 wrBuf[4] ��
    {
        crc = AD7175_ComputeCRC(wrBuf, reg.size+1);
        wrBuf[reg.size + 1] = ~crc;
    }
    /* Write data to the device */                                          //�������ݸ�����
    ret = SPI_Write(AD7175_SLAVE_ID,                                                                        /*  �ӻ���ַ���    */
                    wrBuf,                                                                                          /*  ���ݻ�������    */
                    AD7175_st.useCRC ? reg.size + 2 : reg.size + 1);        /*  ���ʹ����CRCУ�飬�������ݳ���+2���ֽڣ�����+1   */
    return ret;
}
/**************************************************************************//**
* @brief Computes the CRC for a data buffer
* @param pBuf - Data buffer
* @param bufSize - Data buffer size in bytes
* @return Returns the computed CRC
******************************************************************************/
uint8_t AD7175_ComputeCRC(uint8_t* pBuf, uint8_t bufSize)                       //����CRCУ����
{
    uint8_t i = 0;
    uint8_t crc = 0xFF;
    while(bufSize--)
    {
        crc ^= *pBuf++;
        for(i = 0; i < 8; i++)
        {
            if(crc & 0x80)
            {
                crc = (crc << 1) ^ AD7175_CRC_POLYNOMIAL;
            }
            else
            {
                crc <<= 1;
            }
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
    {
        /* Read the value of the Status Register */
        ret = AD7175_ReadRegister(&AD7175_regs[Status_Register]);
        if(ret < 0)
        {
            return ret;
        }
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
    /*  ��ȡ����Ĵ���  */
    /*  ��ȡ��ɣ����ݱ�����    �ṹ�������Ӧ��Ԫ���У���������ݰ���{addr,Value,byte_num}     */
    ret = AD7175_ReadRegister(&AD7175_regs[Data_Register]);
    /* Get the read result */
    /*  �ӽṹ������Ԫ���У���ȡ��Ӧ�Ľ��ֵ    */
//   pData = AD7175_regs[Data_Register].value;
    return ret;
}

void Init_AD7175(void)
{
	int8_t Flg_Ready;
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



int32_t Get_ADC_Reault_ADC_average(u16 CH_num,u16 setup_cfg)
{
	int32_t a[10]={0};
	int32_t xx=0,yy;
	int8_t i=0,j=0;
	int32_t  ADC_average;

	AD7175_regs[CH_Map_1].value=CH_num;
    AD7175_regs[Setup_Config_1].value=setup_cfg; 
	SPI_CS_0();  
	AD7175_WriteRegister(AD7175_regs[CH_Map_1]);
	AD7175_WriteRegister(AD7175_regs[Setup_Config_1]);
	
	for(i=0;i<10;i++)
	{
		AD7175_ReadRegister(&AD7175_regs[Data_Register]); 
		a[i]= AD7175_regs[Data_Register].value; 
	}
	SPI_CS_1(); 
//*******From max to min: a[0]=max.....a[9]=min

	for(i=0;i<10;i++)
	{
		for(j=9;j>i;j--)
		{
			if(a[j]>a[j-1])
			{
				xx=a[j-1];
				a[j-1]=a[j];
				a[j]=xx;
			}
		}
	}
	/*
	********Delete max and min, other 8 nums add and average
	*/
	for(i=1;i<9;i++)
	{
		yy+=a[i];
	}
	ADC_average=(yy+0.00)/8.0;
	return ADC_average;
}
//============================================================================
//8001      AIN0_AIN1
//8044      AIN2_AIN4
//8064      AIN3_AIN4
//8056      AIN2_REF-
//1300      ˫����
//0300      ������
void Get_ADC_Reault(u16 CH_num,u16 setup_cfg)
{
    AD7175_regs[CH_Map_1].value=CH_num;
    AD7175_regs[Setup_Config_1].value=setup_cfg; 
	SPI_CS_0();  
	AD7175_WriteRegister(AD7175_regs[CH_Map_1]);
	AD7175_WriteRegister(AD7175_regs[Setup_Config_1]);
	
	AD7175_ReadRegister(&AD7175_regs[Data_Register]); 

    SPI_CS_1();
}

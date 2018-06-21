
#include "main.h"
AK8963_T g_tMag;		

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitAK8963
*	����˵��: ��ʼ��AK8963
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitAK8963(void)
{
	//TWI_init();
		AK8963_WriteByte(AK8963_CNTL2,0x01);//��λ����
		AK8963_WriteByte(AK8963_CNTL1,0x16);//����Ϊ16bitģʽ ������Ϊ100HZ	
		g_tMag.CfgRegA = AK8963_ReadByte(AK8963_WIA);
		printf("ID=0x%x\r\n",g_tMag.CfgRegA);
}

//��ȡ����
void GetAK8963Mag(void)
{
		AK8963_WriteByte(AK8963_CNTL2,0x01);//��λоƬ
		AK8963_WriteByte(AK8963_CNTL1,0x16);//����Ϊ16bitģʽ ������Ϊ100HZ	
		//delayms(5);
		while(!AK8963_ReadByte(AK8963_ST1));//�ȵ�����ת�����
		AK8963_ReadData();//������ȡ��������ݣ������ڽṹ����
//		printf("X_Mag=%d,Y_Mag=%d,Z_Mag=%d\r\n",g_tMag.X,g_tMag.Y,g_tMag.Z);
}

//���Ҳ���ģʽ
uint8_t Self_test(void)
{
		AK8963_WriteByte(AK8963_CNTL2,0x01);
		AK8963_WriteByte(AK8963_CNTL1,0x00);//power down
		AK8963_WriteByte(AK8963_ASTC,0x40);//Set self test
		AK8963_WriteByte(AK8963_CNTL1,0x18);//SET Self Mode
		while(!AK8963_ReadByte(AK8963_ST1));
		AK8963_ReadData();
		//usart4.printf("X_Mag=%d,Y_Mag=%d,Z_Mag=%d\r\n",g_tMag.X,g_tMag.Y,g_tMag.Z);
		AK8963_WriteByte(AK8963_ASTC,0x00);//Clear self test
		AK8963_WriteByte(AK8963_CNTL1,0x00);//power down
		AK8963_WriteByte(AK8963_CNTL2,0x01);
		if(((g_tMag.X>=-200)&&(g_tMag.X<=200))&&((g_tMag.Y>=-200)&&(g_tMag.Y<=200))&&((g_tMag.Z>=-3200)&&(g_tMag.Z<=-800)))
			return 0;
		else return 1;
}
/*
*********************************************************************************************************
*	�� �� ��: AK8963_WriteByte
*	����˵��: �� AK8963 �Ĵ���д��һ������
*	��    ��: _ucRegAddr : �Ĵ�����ַ
*			  _ucRegData : �Ĵ�������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void AK8963_WriteByte(uint8_t _ucRegAddr, uint8_t _ucRegData)
{
    TWI_Start();							/* ���߿�ʼ�ź� */
    TWI_Write(AK8963_SLAVE_ADDRESS);	/* �����豸��ַ+д�ź� */
    TWI_Write(_ucRegAddr);				/* �ڲ��Ĵ�����ַ */
    TWI_Write(_ucRegData);				/* �ڲ��Ĵ������� */
    TWI_Stop();                   			/* ����ֹͣ�ź� */
}

/*
*********************************************************************************************************
*	�� �� ��: AK8963_ReadByte
*	����˵��: ��ȡ MPU-6050 �Ĵ���������
*	��    ��: _ucRegAddr : �Ĵ�����ַ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
uint8_t AK8963_ReadByte(uint8_t _ucRegAddr)
{
	uint8_t ucData;

	TWI_Start();                  			/* ���߿�ʼ�ź� */
	TWI_Write(AK8963_SLAVE_ADDRESS);	/* �����豸��ַ+д�ź� */
	TWI_Write(_ucRegAddr);     			/* ���ʹ洢��Ԫ��ַ */
	TWI_Start();                  			/* ���߿�ʼ�ź� */
	TWI_Write(AK8963_SLAVE_ADDRESS+1); 	/* �����豸��ַ+���ź� */
	ucData = i2c_read(0);
	TWI_Stop();                  			/* ����ֹͣ�ź� */
	return ucData;
}

/*
*********************************************************************************************************
*	�� �� ��: AK8963_ReadData
*	����˵��: ��ȡ MPU-6050 ���ݼĴ����� ���������ȫ�ֱ��� g_tMag.  ��������Զ�ʱ���øó���ˢ������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void AK8963_ReadData(void)
{
	uint8_t ucReadBuf[6];
	uint8_t i;

#if 1 /* ������ */
	TWI_Start();                  			/* ���߿�ʼ�ź� */
	TWI_Write(AK8963_SLAVE_ADDRESS);	/* �����豸��ַ+д�ź� */
	//WaitACK();
	TWI_Write(DATA_OUT_X);     		/* ���ʹ洢��Ԫ��ַ  */
	//WaitACK();

	TWI_Start();                  			/* ���߿�ʼ�ź� */

	TWI_Write(AK8963_SLAVE_ADDRESS + 1); /* �����豸��ַ+���ź� */
	//WaitACK();

	for (i = 0; i < 5; i++)
	{
		ucReadBuf[i] = i2c_read(1);       			/* �����Ĵ������� */
		//i2c_Ack();
	}

	/* �����һ���ֽڣ�ʱ�� NAck */
	ucReadBuf[5] = i2c_read(0);
	//i2c_NAck();

	TWI_Stop();                  			/* ����ֹͣ�ź� */

#else	/* ���ֽڶ� */
	for (i = 0 ; i < 7; i++)
	{
		ucReadBuf[i] = AK8963_ReadByte(DATA_OUT_X + i);
	}
#endif

	/* �����������ݱ��浽ȫ�ֽṹ����� */
	g_tMag.X = (int16_t)((ucReadBuf[1] << 8) + ucReadBuf[0]);
	g_tMag.Y = (int16_t)((ucReadBuf[3] << 8) + ucReadBuf[2]);
	g_tMag.Z = (int16_t)((ucReadBuf[5] << 8) + ucReadBuf[4]);
	
	
//	/* ͳ�����ֵ����Сֵ */
//	if ((g_tMag.X > - 2048) && (g_tMag.X < 2048))
//	{
//		if (g_tMag.X > g_tMag.X_Max)
//		{
//			g_tMag.X_Max = g_tMag.X;
//		}
//		if (g_tMag.X < g_tMag.X_Min)
//		{
//			g_tMag.X_Min = g_tMag.X;
//		}	
//	}

//	if ((g_tMag.Y > - 2048) && (g_tMag.Y < 2048))
//	{
//		if (g_tMag.Y > g_tMag.Y_Max)
//		{
//			g_tMag.Y_Max = g_tMag.Y;
//		}
//		if (g_tMag.Y < g_tMag.Y_Min)
//		{
//			g_tMag.Y_Min = g_tMag.Y;
//		}	
//	}
//	
//	if ((g_tMag.Z > - 2048) && (g_tMag.Z < 2048))
//	{
//		if (g_tMag.Z > g_tMag.Z_Max)
//		{
//			g_tMag.Z_Max = g_tMag.Z;
//		}
//		if (g_tMag.Z < g_tMag.Z_Min)
//		{
//			g_tMag.Z_Min = g_tMag.Z;
//		}	
//	}
}




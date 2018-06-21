
#include "main.h"
AK8963_T g_tMag;		

/*
*********************************************************************************************************
*	函 数 名: bsp_InitAK8963
*	功能说明: 初始化AK8963
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitAK8963(void)
{
	//TWI_init();
		AK8963_WriteByte(AK8963_CNTL2,0x01);//复位器件
		AK8963_WriteByte(AK8963_CNTL1,0x16);//设置为16bit模式 采样率为100HZ	
		g_tMag.CfgRegA = AK8963_ReadByte(AK8963_WIA);
		printf("ID=0x%x\r\n",g_tMag.CfgRegA);
}

//获取数据
void GetAK8963Mag(void)
{
		AK8963_WriteByte(AK8963_CNTL2,0x01);//复位芯片
		AK8963_WriteByte(AK8963_CNTL1,0x16);//设置为16bit模式 采样率为100HZ	
		//delayms(5);
		while(!AK8963_ReadByte(AK8963_ST1));//等等数据转换完成
		AK8963_ReadData();//连续读取三轴的数据，保存在结构体中
//		printf("X_Mag=%d,Y_Mag=%d,Z_Mag=%d\r\n",g_tMag.X,g_tMag.Y,g_tMag.Z);
}

//自我测试模式
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
*	函 数 名: AK8963_WriteByte
*	功能说明: 向 AK8963 寄存器写入一个数据
*	形    参: _ucRegAddr : 寄存器地址
*			  _ucRegData : 寄存器数据
*	返 回 值: 无
*********************************************************************************************************
*/
void AK8963_WriteByte(uint8_t _ucRegAddr, uint8_t _ucRegData)
{
    TWI_Start();							/* 总线开始信号 */
    TWI_Write(AK8963_SLAVE_ADDRESS);	/* 发送设备地址+写信号 */
    TWI_Write(_ucRegAddr);				/* 内部寄存器地址 */
    TWI_Write(_ucRegData);				/* 内部寄存器数据 */
    TWI_Stop();                   			/* 总线停止信号 */
}

/*
*********************************************************************************************************
*	函 数 名: AK8963_ReadByte
*	功能说明: 读取 MPU-6050 寄存器的数据
*	形    参: _ucRegAddr : 寄存器地址
*	返 回 值: 无
*********************************************************************************************************
*/
uint8_t AK8963_ReadByte(uint8_t _ucRegAddr)
{
	uint8_t ucData;

	TWI_Start();                  			/* 总线开始信号 */
	TWI_Write(AK8963_SLAVE_ADDRESS);	/* 发送设备地址+写信号 */
	TWI_Write(_ucRegAddr);     			/* 发送存储单元地址 */
	TWI_Start();                  			/* 总线开始信号 */
	TWI_Write(AK8963_SLAVE_ADDRESS+1); 	/* 发送设备地址+读信号 */
	ucData = i2c_read(0);
	TWI_Stop();                  			/* 总线停止信号 */
	return ucData;
}

/*
*********************************************************************************************************
*	函 数 名: AK8963_ReadData
*	功能说明: 读取 MPU-6050 数据寄存器， 结果保存在全局变量 g_tMag.  主程序可以定时调用该程序刷新数据
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void AK8963_ReadData(void)
{
	uint8_t ucReadBuf[6];
	uint8_t i;

#if 1 /* 连续读 */
	TWI_Start();                  			/* 总线开始信号 */
	TWI_Write(AK8963_SLAVE_ADDRESS);	/* 发送设备地址+写信号 */
	//WaitACK();
	TWI_Write(DATA_OUT_X);     		/* 发送存储单元地址  */
	//WaitACK();

	TWI_Start();                  			/* 总线开始信号 */

	TWI_Write(AK8963_SLAVE_ADDRESS + 1); /* 发送设备地址+读信号 */
	//WaitACK();

	for (i = 0; i < 5; i++)
	{
		ucReadBuf[i] = i2c_read(1);       			/* 读出寄存器数据 */
		//i2c_Ack();
	}

	/* 读最后一个字节，时给 NAck */
	ucReadBuf[5] = i2c_read(0);
	//i2c_NAck();

	TWI_Stop();                  			/* 总线停止信号 */

#else	/* 单字节读 */
	for (i = 0 ; i < 7; i++)
	{
		ucReadBuf[i] = AK8963_ReadByte(DATA_OUT_X + i);
	}
#endif

	/* 将读出的数据保存到全局结构体变量 */
	g_tMag.X = (int16_t)((ucReadBuf[1] << 8) + ucReadBuf[0]);
	g_tMag.Y = (int16_t)((ucReadBuf[3] << 8) + ucReadBuf[2]);
	g_tMag.Z = (int16_t)((ucReadBuf[5] << 8) + ucReadBuf[4]);
	
	
//	/* 统计最大值和最小值 */
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




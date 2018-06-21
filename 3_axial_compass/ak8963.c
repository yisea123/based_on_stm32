

#include "main.h"
uchar SeptSouthNorth;
//long X_Dat,Y_Dat,Z_Dat;
long Ex_X_Dat,Ex_Y_Dat,Ex_Z_Dat;
int	X_Data,Y_Data,Z_Data;
float X_Calibrate,Y_Calibrate,Z_Calibrate;

long South_X,South_Y,South_Z;
long North_X,North_Y,North_Z;
uchar a[12];

void AK8963_init(void)
{
	AK8963_Wirte(0x0b,0x01);	
	AK8963_Wirte(0x0a,0x12);
	AK8963_Wirte(0x0b,0x00);
	AK8963_Wirte(0x0c,0x00);
	AK8963_Wirte(0x0f,0x00);
	
	AK8963_Wirte(0x0a,0x1f);
	a[11] = AK8963_Read(0x12);
	a[10] = AK8963_Read(0x11);
	a[9] = AK8963_Read(0x10);
	
//	printf("FtoC%d%d%d",a[10],a[11],a[12]);
	AK8963_Wirte(0x0b,0x01);	
	AK8963_Wirte(0x0a,0x12);
	AK8963_Wirte(0x0b,0x00);
	AK8963_Wirte(0x0c,0x00);
	AK8963_Wirte(0x0f,0x00);
	
	X_Calibrate = (a[9]-128)*0.5/128+1;
	Y_Calibrate = (a[9]-128)*0.5/128+1;
	Z_Calibrate = (a[9]-128)*0.5/128+1;
}

void AK8963_Wirte(uchar addr, uchar dat)
{
     TWI_Start();
	 TWI_Write(AK8963_add);
	 TWI_Write(addr);
	 TWI_Write(dat);
	 TWI_Stop();
	 delayms(1);
}

uchar AK8963_Read(uchar addr)
{
 	 uchar data;
	 TWI_Start();
	 TWI_Write(AK8963_add); 
	 TWI_Write(addr);
	 
	 TWI_Start();
	 TWI_Write(AK8963_add | 0x01); 
	 data = i2c_read(0);
	 TWI_Stop();
	 return data;
}

void AK8963_Read_Data(void)
{ 
	 AK8963_Wirte(0x0a,0x12);
	 //if(INT)
	if((AK8963_Read(0x02)&0x01))
	 {
	  	//while(!(AK8963_Read(0x02)&0x01));
		a[0] = AK8963_Read(0x03);
	    a[1] = AK8963_Read(0x04);
		a[2] = AK8963_Read(0x05);
	    a[3] = AK8963_Read(0x06);
		a[4] = AK8963_Read(0x07);
	    a[5] = AK8963_Read(0x08);
	//	a[6] = AK8963_Read(0x09);
	 }
	// else
	//  	Transmit_arraydata("AK8963 Read Data Fail!\r\n"); 
	// AK8963_Wirte(0x0a,0x10);
}

//=======================================================
uint AK8963_Data_Processing(void)
{
	long temp;
	int TempH,TempL;
//	int	X_Data,Y_Data,Z_Data;
//	GetAK8963Mag();
//	print/f("X_Mag=%d,Y_Mag=%d,Z_Mag=%d\r\n",g_tMag.X,g_tMag.Y,g_tMag.Z);
//	X_Data = g_tMag.X;
//	Y_Data = g_tMag.Y;
//	Z_Data = g_tMag.Z;
	AK8963_Smoothing(30,1);
	Ex_X_Dat = 1.5*X_Data*X_Calibrate;
	Ex_Y_Dat = 1.5*Y_Data*Y_Calibrate;
	Ex_Z_Dat = 1.5*Z_Data*Z_Calibrate;
	switch(SeptSouthNorth)
	{
		case 0:
			South_X = Ex_X_Dat;
			South_Y = Ex_Y_Dat;
			South_Z = Ex_Z_Dat;
			SeptSouthNorth ++;
			break;
		case 1:
			North_X = Ex_X_Dat;
			North_Y = Ex_Y_Dat;
			North_Z = Ex_Z_Dat;
			SeptSouthNorth = 0;
			break;
	}
	
	//XSData = Ex_X_Dat;
	//YSData = Ex_Y_Dat;
	//ZSData = Ex_Z_Dat;
//	Ex_X_Dat = 1.5*X_Data*((float)(a[9]-128)*0.5/128+1);
//	Ex_Y_Dat = 1.5*Y_Data*((float)(a[10]-128)*0.5/128+1);
//	Ex_Z_Dat = 1.5*Z_Data*((float)(a[11]-128)*0.5/128+1);
	
	
//	Ex_X_Dat = (double)X_Data*1.5;
//	Ex_Y_Dat = (double)Y_Data*1.5;
//	Ex_Z_Dat = (double)Z_Data*1.5;
//	printf("g_tMag.X %d\r\n",Ex_X_Dat);
//	printf("g_tMag.Y %d\r\n",Ex_Y_Dat);
//	printf("g_tMag.Z %d\r\n",Ex_Z_Dat);
	return 0;
}
//======================================================
uint AK8963_Smoothing(uchar time,uint ms)
{
	int X_AK8963_max=0,X_AK8963_min=0xffff,X_AK8963_data=0;
	int Y_AK8963_max=0,Y_AK8963_min=0xffff,Y_AK8963_data=0;
	int Z_AK8963_max=0,Z_AK8963_min=0xffff,Z_AK8963_data=0;
	uchar i;
	long X_AK8963_sum=0,Y_AK8963_sum=0,Z_AK8963_sum=0;
	for(i=0;i<time;i++)
	{
		GetAK8963Mag();
		//AK8963_Data_Processing();
//		printf("Ex_X_Dat is %d\r\n",g_tMag.X);
//		printf("Ex_Y_Dat is %d\r\n",g_tMag.Y);
//		printf("Ex_Z_Dat is %d\r\n",g_tMag.Z);
		X_AK8963_data = g_tMag.X;
		Y_AK8963_data = g_tMag.Y;
		Z_AK8963_data = g_tMag.Z;
		
		if(X_AK8963_data > X_AK8963_max) X_AK8963_max = X_AK8963_data;
		if(X_AK8963_data < X_AK8963_min) X_AK8963_min = X_AK8963_data;
		X_AK8963_sum += X_AK8963_data;
		X_Data = ((X_AK8963_sum - X_AK8963_max - X_AK8963_min)/(time - 2));
		
		if(Y_AK8963_data > Y_AK8963_max) Y_AK8963_max = Y_AK8963_data;
		if(Y_AK8963_data < Y_AK8963_min) Y_AK8963_min = Y_AK8963_data;
		Y_AK8963_sum += Y_AK8963_data;
		Y_Data = ((Y_AK8963_sum - Y_AK8963_max - Y_AK8963_min)/(time - 2));
		
		if(Z_AK8963_data > Z_AK8963_max) Z_AK8963_max = Z_AK8963_data;
		if(Z_AK8963_data < Z_AK8963_min) Z_AK8963_min = Z_AK8963_data;
		Z_AK8963_sum += Z_AK8963_data;
		Z_Data = ((Z_AK8963_sum - Z_AK8963_max - Z_AK8963_min)/(time - 2));
		delayms(ms);
	}
	if(g_tMag.Y == 6425)
	{
		X_Data = 0;
		Y_Data = 0;
		Z_Data = 0;
		printf("SENSOR IIC ERROR\r\n"); 
		return 0;
	}
	return 0; 
}
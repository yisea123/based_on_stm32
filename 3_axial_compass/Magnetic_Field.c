//2015-1-13,glp
//#include "macros.h"
//#include <iom64v.h> 
//#include "AD5660.h"
#include "main.h"

//uint magnetic_ratio;
//uint magnetic_offset;
uchar Flg_Updata = 1;
uchar Step_Key_Up;
uchar Step_Key_Down;
float Key_Tem;
uchar Count;
uchar FastTime_Key;
uchar Flg_CloseMagnetic;
long XSData,YSData,ZSData;
uint AD_sample_X,AD_sample_Y,AD_sample_Z;
long NMS_sample_X,NMS_sample_Y,NMS_sample_Z;



void Magnetic_Field(uchar Sw,uchar Io)
{
	float DA_Data;
	
	if(Flg_Updata == 1)
	{
		DA_Data = Float_EEPROM_Read(Addr_Current);
		Cotrol_Coil_Current(DA_Data,1);
		Flg_Updata = 0;
	}
	if(Sw == 'N')
	{
		if(Io == 1)
		{ 
			Flg_CloseMagnetic = 1;
			Clr_North_Magnetic;
			Clr_South_Magnetic;
			delayms(500);
			Set_North_Magnetic;
		}
		else
		{
		 	Flg_CloseMagnetic = 0;
			Clr_North_Magnetic;
			Clr_South_Magnetic;
		}
		
	}
	else if(Sw == 'S')
	{
		if(Io == 1)
		{
			Flg_CloseMagnetic = 1;
			Clr_North_Magnetic;
			Clr_South_Magnetic;
			delayms(500);
			Set_South_Magnetic;
			Set_North_Magnetic;
		}
		else
		{
			Flg_CloseMagnetic = 0;
			Clr_North_Magnetic;
			Clr_South_Magnetic;
		}
	}
}
//==============================================================
void Cotrol_Coil_Current(float Current,uchar a)
{
	unsigned short DA_Value;
	float b;
	if(Current > 280)
		Set_HightV1;
	else if(Current > 230)
		Set_HightV2;
	else if(Current > 150)
		Set_HightV3;	
	else 
		Set_HightV4;
//	printf("Current111 is:%d\r\n",Current);
//	
	b = Float_EEPROM_Read(AddrF_DAOff);
//	printf("Current111 is:%f\r\n",b);
	DA_Value = (65535*(Current+b))/5000;
	ad5660_wr_buf[2]=0;
	ad5660_wr_buf[1]=DA_Value>>8;
	ad5660_wr_buf[0]=DA_Value;
	Wr_AD5660(ad5660_wr_buf,3);
	if(a != 0)
	{
		Float_EEPROM_write(Addr_Current,Current);
		//EEPROM_write(0x01,(Current));
		//EEPROM_write(0x00,(Current>>8));
	}
}
//================================================================
/*
void Print_XYZ_NMS(void)
{
	uint Tem_Valu;
	Tem_Valu = X_Dat*magnetic_ratio + magnetic_offset;
	Transmit_arraydata("NMS_sample[0]:");
	SendData2(Tem_Valu);
	Tem_Valu = Y_Dat*magnetic_ratio + magnetic_offset;
	Transmit_arraydata("NMS_sample[1]:");
	SendData2(Tem_Valu);
	Tem_Valu = Z_Dat*magnetic_ratio + magnetic_offset;
	Transmit_arraydata("NMS_sample[2]:");
	SendData2(Tem_Valu);
}
*/
//==========================================

void Key_Control_Up(void)
{
	switch(Step_Key_Up)
	{
		case 0:
			Flg_Updata = 1;
			Key_Tem = Float_EEPROM_Read(Addr_Current);
			Step_Key_Up ++;
		break;
		case 1:
			Key_Tem = ++Key_Tem; 
			if(Key_Tem > 1000)
				Key_Tem = 1000;
			Cotrol_Coil_Current(Key_Tem,1);
			delayms(500);
			Count ++;
			if(Count > 3)
			{
				Count = 0;
				Step_Key_Up ++;
			}	
		break;
		case 2:
			delayms(50);
			Key_Tem = ++Key_Tem; 
			if(Key_Tem > 1000)
				Key_Tem = 1000;
			Cotrol_Coil_Current(Key_Tem,1);
		break;
	}	
}
//===================================================================
void Key_Control_Down(void)
{
	switch(Step_Key_Down)
	{
		case 0:
			Flg_Updata = 1;
			Key_Tem = Float_EEPROM_Read(Addr_Current);
			Step_Key_Down ++;
		break;
		case 1:
			Key_Tem = --Key_Tem; 
			if(Key_Tem < 1)
				Key_Tem = 1;
			Cotrol_Coil_Current(Key_Tem,1);
			delayms(500);
			Count ++;
			if(Count > 3)
			{
				Count = 0;
				Step_Key_Down ++;
			}	
		break;
		case 2:
			delayms(50);
			Key_Tem = --Key_Tem; 
			if(Key_Tem > 1000)
				Key_Tem = 1000;
			Cotrol_Coil_Current(Key_Tem,1);
		break;
	}	
}

//=============================================================
float Read_Current(void)
{
	float a,b;
	long c;
	if(Flg_CloseMagnetic == 0)
	{
		a = 0;
		return a;
	}
	ads1110Config();
	delayms(2);
	b = Float_EEPROM_Read(AddrF_CurrOff);

//	printf("b is%f\r\n",b);
	a = get_ad_result();
	a = (a*2048)/131072 + b;
	if(a > 600)
		a = 0;
//	printf("valu %f\r\n",a);
	return a;
}
//===================================================

long Read_NMS(uchar Switch_NS)
{
	uint Tem_uchar;
	long North,South;
	long NMS_Magnetic;
//	
	long temp1,temp2,temp3;
	double Xtemp,Ytemp,Ztemp;
	long TemSq;
	float Tem1_float;
	if(Switch_NS == 1)
	{
		Magnetic_Field('S',1);
		delayms(100);
		AK8963_Data_Processing();
	}
	
//	Tem1_float = Read_Current();
//	printf("current1 %f\r\n",Tem1_float);

	
	
	//printf("XSData %d\r\n",XSData);
	//printf("YSData %d\r\n",YSData);
	//printf("ZSData %d\r\n",ZSData);
	//Magnetic_Field('N',0);
	//delayms(100);
	if(Switch_NS == 1)
	{
		Magnetic_Field('N',1);
		delayms(100);
		AK8963_Data_Processing();
	}	
	//South = AK8963_Smoothing(30,1);
	//printf("XNData %d\r\n",Ex_X_Dat);
	//printf("YNData %d\r\n",Ex_Y_Dat);
	//printf("ZNData %d\r\n",Ex_Z_Dat);
	
	temp1 = (South_X-North_X);
	temp2 = (South_Y-North_Y);
	temp3 = (South_Z-North_Z);
	
	South_X = 0;
	South_Y = 0;
	South_Z = 0;
	North_X = 0;
	North_Y = 0;
	North_Z = 0;
	
//	printf("XS-N %d\r\n",temp1);
//	printf("YS-N %d\r\n",temp2);
//	printf("ZS-N %d\r\n",temp3);

	Xtemp = pow(temp1,2);
	Ytemp = pow(temp2,2);
	Ztemp = pow(temp3,2);

//	printf("SumTemX %lf\r\n",Xtemp);
//	printf("SumTemY %lf\r\n",Ytemp);
//	printf("SumTemZ %lf\r\n",Ztemp);
	
	NMS_Magnetic = sqrt(Xtemp+Ytemp+Ztemp);

	return NMS_Magnetic;
}
void Calculate_Sample_magnetic(void)
{
	ulong I1,I2,I3;
	ulong North,South;
	ulong NMS_Magnetic_A,NMS_Magnetic_B;
	float FixRito;
	float A,B;
	
	
	FixRito = Float_24C02_Read(AddrF_FixRat);
//	printf("FixRito is:%f\r\n",FixRito);
		
	I1 = E24C02_Read_Uint(Sample1000);
	I2 = E24C02_Read_Uint(Sample3500);
	I1 = 1000;
	I2 = 3500;

	I1 = I1/FixRito;//i1 = 1000/fixture_rito
	I2 = I2/FixRito;//i2 = 3500/fixture_rito
	I3 = 3000/FixRito;//i2 = 3500/fixture_rito
	E24C02_Write_Uint(Currer_I1,I1);
	E24C02_Write_Uint(Currer_I2,I2);
	E24C02_Write_Uint(Currer_I3,I3);
	
//	I1 = 88;
//	I2 = 308;
//	I3 = 264;
//	printf("I1 is:%d\r\n",I1);
//	printf("I2 is:%d\r\n",I2);

	AD_sample_X = I1;
	AD_sample_Y = I3;
	AD_sample_Z = I2;
//	printf("AD_sample[0]:%d\r\n",AD_sample_X);
//	printf("AD_sample[1]:%d\r\n",AD_sample_Y);
//	printf("AD_sample[2]:%d\r\n",AD_sample_Z);
		
	Flg_Updata = 0;	
	Cotrol_Coil_Current(I1,0);
	NMS_Magnetic_A = Read_NMS(1);
	//NMS_sample_X = NMS_Magnetic_A; 
	
	Cotrol_Coil_Current(I2,0);
	NMS_Magnetic_B = Read_NMS(1);
	//NMS_sample_Z = NMS_Magnetic_B; 
	//I2 = Read_Current();
	
	A = (3500-1000)/(double)((NMS_Magnetic_B - NMS_Magnetic_A));
	B = 1000 - A*NMS_Magnetic_A;

	printf("A is:%f\r\n",A);
	printf("B is:%f\r\n",B);
	Float_24C02_write(AddrF_A,A);
	Float_24C02_write(AddrF_B,B);
	Flg_Updata = 1;	
}


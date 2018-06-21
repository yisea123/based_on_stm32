
#include "main.h"
uchar x[4];

//=================================================
void EEPROM_Init(void)
{
	if(EEPROM_Read_Uint(Addr_First_Time) != 111) 
	{
		Float_EEPROM_write(AddrF_DAOff,0);
		Float_EEPROM_write(AddrF_CurrOff,0);
		Float_EEPROM_write(Addr_Current,264);
		
		EEPROM_Write_Uint(Addr_First_Time,111);
	}
	if(E24C02_Read_Uint(Addr_First_Time) != 111) 
	{
		EEPROM_Write_Uint(Addr_Current,0x00);
		EEPROM_Write_Uint(Addr_Ratio_Gs,0x00);

		Float_24C02_write(AddrF_FixRat,0);
		Float_24C02_write(AddrF_A,0);
		Float_24C02_write(AddrF_B,0);
		
		E24C02_Write_Uint(Addr_First_Time,111);
	}
}
//=======================================================
void FtoC(float a)
{ 
	uchar i,*px;
//	uchar x[4]; 
	void *pf;
	px=x;
	pf=&a;
	for(i=0;i<4;i++)
	{
		 *(px+i)=*((char *)pf+i); 
	} 
}
float CtoF(void)
{ 
	float a,b;
	uchar i,*px;
//	uchar x[4]={56,180,150,73};
	void *pf;
	px=x;
	pf=&a;
	for(i=0;i<4;i++)
	{
		*((char *)pf+i)=*(px+i);
	}
	return a;
}
//====================================================================
void EEPROM_Write_Uint(uchar addr,uint valu)
{
	EEPROM_write(addr+1,valu);
	EEPROM_write(addr,(valu>>8));
}
uint EEPROM_Read_Uint(uchar addr)
{
	uint j;
	j = EEPROM_read(addr);
	j = j<<8;
	j |= EEPROM_read(addr+1);
	return j;
}
//=================================================
void Float_EEPROM_write(uchar Addr,float a)
{
	FtoC(a);
	//printf("FtoC%x%x%x%x",x[0],x[1],x[2],x[3]);
	EEPROM_write((Addr+0),x[0]);
	EEPROM_write((Addr+1),x[1]);
	EEPROM_write((Addr+2),x[2]);
	EEPROM_write((Addr+3),x[3]);
}
float Float_EEPROM_Read(uchar Addr)
{
	float a;
	x[0] = EEPROM_read(Addr+0);
	x[1] = EEPROM_read(Addr+1);
	x[2] = EEPROM_read(Addr+2);
	x[3] = EEPROM_read(Addr+3);
	//printf("FtoC%x%x%x%x",x[0],x[1],x[2],x[3]);
	a  = CtoF();
	return a;
}
//========================================
void E24C02_Write_Uint(uint addr,uint valu)
{
	x[0] = (valu>>8);
	x[1] = valu;
	wt24c(x,addr,2);
}
uint E24C02_Read_Uint(uint addr)
{
	uint j;
	rd24c(x,addr,2);
	j = x[0];
	j = (j<<8);
	j |= x[1];
	
	return j;
}
//=================================================
void Float_24C02_write(uint addr,float a)
{
	FtoC(a);
	wt24c(x,addr,4);
}
float Float_24C02_Read(uint addr)
{
	float a;
	rd24c(x,addr,4);
	a  = CtoF();
	return a;
}
//====================================================


/*
//=================================================
void E24C02_Write(uchar Addr,char a)
{
	TWI_Start();
	TWI_Write(0xa0);
	TWI_Write(Addr);
	TWI_Write(a);
	TWI_Stop();
}
uchar E24C02_read(uchar Addr)
{
	uchar date;
	TWI_Start();
	TWI_Write(0xa0);
	TWI_Write(Addr);
	TWI_Start();
	TWI_Write(0xa1);
	date = i2c_read(0);
	TWI_Stop();
	return date;	
}
//======================================================
void Float_E24C02_write(uchar Addr,float a)
{
	FtoC(a);
	//printf("FtoC%x%x%x%x",x[0],x[1],x[2],x[3]);
	E24C02_Write((Addr+0),x[0]);
	E24C02_Write((Addr+1),x[1]);
	E24C02_Write((Addr+2),x[2]);
	E24C02_Write((Addr+3),x[3]);
}
float Float_E24C02_Read(uchar Addr)
{
	float a;
	x[0] = E24C02_read(Addr+0);
	x[1] = E24C02_read(Addr+1);
	x[2] = E24C02_read(Addr+2);
	x[3] = E24C02_read(Addr+3);
	//printf("FtoC%x%x%x%x",x[0],x[1],x[2],x[3]);
	a  = CtoF();
	return a;
}
*/
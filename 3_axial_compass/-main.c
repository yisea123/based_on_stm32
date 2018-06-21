// Target : M64
// Crystal: 11.0592M
// change time 2014-07-11


#include "main.h"
#include"Commands.H"

//uchar Flg_South;
//uchar Flg_CloseKey_S,Flg_CloseKey_N;
//uchar Flg_KeyOneTime_N,Flg_KeyOneTime_S;
//uchar Flg_OpenRelay;
//uchar Flg_North;
uchar LED_Time_Flg;
ulong Tem_Valu;
//unsigned int ControlDataTab[3];
//unsigned int    ADCTem;
unsigned int	LED_Time;
//unsigned int	key;
//unsigned int 	ReadData;
//unsigned long int	Countkey;
//long unsigned int	CountCY;
//uchar	AD5160_Valu;
//uchar Flg_North_New	= 0;
//uchar Flg_South_New = 0;
uchar FID_Buffer[35];
unsigned long Cont;

//uchar cmd_index_temp=0;
//int QT_KEY_flag=0;
//uchar SIM_KEY=0;
//int AUT_flag=0, AUT_IN_flag, AUT_OUT_flag;

uchar	LockButton_Flg;
uchar	TestRunningFlg;
uchar	LED_Time_Flg;
uchar	step = 0;
//uchar  Flg_MLB_On;
//uchar  Flg_USB_On;

void SendData(unsigned char dat);
unsigned short getNunb(char *Point);
void Valu_Save(ulong i);
uint Valu_Read(void);
/*
ulong Adjust(ulong Value)
{
   ulong A=Value;
   if((A>290)&&(A<=320)) A+=15;
   return A;
}
*/
void SendData2(unsigned short dat)
{
unsigned short Val;
uchar G, S, B, Q, W;
unsigned char flg=0;
	Val = dat;
	W = Val/10000%10;
	Q  = Val/1000%10;
	B = Val/100%10;
	S = Val/10%10;
	G = Val%10;
	if(W != 0)  
	{
	 	 USART_Transmit_byte(0X30 + W);
		 flg = 1;
	}
	if((Q != 0)||(flg==1))   
	{
	 	 USART_Transmit_byte(0X30 + Q);
		 flg = 1;
	}
	if((B != 0)||(flg==1))   
	{
	 	 USART_Transmit_byte(0X30 + B);
		 flg = 1;
	}
	if((S != 0)||(flg==1))   
	{
	 	 USART_Transmit_byte(0X30 + S);
	}
	USART_Transmit_byte(0X30 + G);
}
/*
void Send_Data(uint dat)
{
   char Buffer_temp[6]={'\0'};
    sprintf(Buffer_temp,"%d",dat);
    Transmit_arraydata(Buffer_temp);
   // USART_Transmit_data_const(r_n);
}
*/
//==============================================================================
void delayms(uint ms)
{
	uint i;
	for(ms;ms>0;ms--)
	  for(i=1590;i>0;i--);
}
void delayus(uint us)
{
	for(us;us>0;us--)
	{
		NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();
	}
}
//==============================================================================

//==============================================================================
void port_init(void)
{

	DDRA = 0xff;
	DDRB = 0xff;
	PORTB &= ~(1<<7)& ~(1<<6)& ~(1<<5)& ~(1<<4);
	DDRC = 0xff;
	DDRD = 0xff; 
	DDRE = 0xff;
	DDRF = 0xff;
	DDRG = 0xff; 
	DDRG &= ~(1<<1)&~(1<<0);
	DDRC &= ~(1<<1)&~(1<<0);
	PORTG |= (1<<1)|(1<<0);
	PORTC |= (1<<1)|(1<<0);
	//DDRF |= (1<<PF0)|(1<<PF1)|(1<<PF2)|(1<<PF3)|(1<<PF4);
    //PORTF |= ((1<<PF0)|(1<<PF1)|(1<<PF2)|(1<<PF3)|(1<<PF4));
    //DDRB &= ~((1<<PB0)|(1<<PB1)|(1<<PB2)|(1<<PB3)|(1<<PB4)|(1<<PB5)|(1<<PB6)|(1<<PB7));
    //PORTB |= (1<<PB0)|(1<<PB1)|(1<<PB2)|(1<<PB3)|(1<<PB4)|(1<<PB5)|(1<<PB6)|(1<<PB7);
}
//==================================================================================
void init_devices(void)
{
	uchar j;
	CLI(); //disable all interrupts
	XDIV  =0x00; //xtal divider
	port_init();
	uart1_init(115200);
	//uart0_init(115200);
	XMCRA =0x00; //external memory
	MCUCR =0x00;
	XMCRB=0x00;
	EICRA =0x00; //extended ext ints
	EICRB =0x00; //extended ext ints
	EIMSK =0x00;
	TIMSK =0x04; //timer interrupt sources
	ETIMSK =0x00; //extended timer interrupt sources
	SEI();       //re-enable interrupts
	for(j=0;j<80;j++)
		CmdBuffer[j] = '\0';
}


//================================================================================
void EEPROM_write(uint address,uchar data)
{
	
	while(EECR & (1<<EEWE));/*等上一次写操作结束*/
	EEAR = address;/*设置地址和数据寄存器*/
	EEDR = data;
	EECR |= (1<<EEMWE);/*置位EEMWE*/
	EECR = (1<<EEWE) | (1<<EEMWE);/*置位EEWE以启动写操作*/
}
uchar EEPROM_read(uint address)
{
	while(EECR & (1<<EEWE));
	EEAR = address;
	EECR |= (1<<EERE); 
	return EEDR;  
}
//===================================================================================
void process(char *s)
{
	int i = 0;
	for(i=0; '\0' != s[i]; i++)
	{ 
		if(s[i] >= 'A' && s[i] <= 'Z')
		s[i] += 32; 
	}
}
//=============================================================================
/**************** 函数功能：对AD5660 进行写数据 入口：要写的数据,十六位 ****************/
void Write_AD5660(unsigned int data)
{
	unsigned char i;
	SCLK2_H;
	NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();
	NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();
	DIN2_H;
	
    SYNC2_H;  // 选择AD5660
	NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();
	NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();
	SYNC2_L;   //使能FSYNC，低电平有效
	NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();
	NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();
	for(i=0;i<24;i++)
	{
	if(data&0x8000)  	DIN2_H
	else  				DIN2_L
	SCLK2_L;
	NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();
	NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();
	SCLK2_H;
	data=data<<1;
	}
	NOP();
	SYNC2_H;
	SCLK2_L;
	NOP();
//	SendData2(data);   
//	Transmit_arraydata("Current");
}
//=============================================================================
void ProgResponseEnd(void)
{
	USART_Transmit_byte(0x0d); 
	USART_Transmit_byte(0x0a);
	Transmit_arraydata(SY);		
}
//==================================================================================

uchar DataCMP(void)
{

	uchar * Data1;
	uchar T_B;
	Data1 = CmdBuffer;
											
	if(strncmp(Data1,Fixture_Information,strlen(Fixture_Information))==0)  							return(1);										 			
	else if(strncmp(Data1,Help,strlen(Help))==0)													return(2); 	
	else if(strncmp(Data1,Reset_Fixture,strlen(Reset_Fixture))==0)									return(3); 
	else if(strncmp(Data1,Code_VER,strlen(Code_VER))==0)											return(4); 
	else if(strncmp(Data1,Open_North_Magnetic_Field,strlen(Open_North_Magnetic_Field))==0)			return(5); 
	else if(strncmp(Data1,Close_North_Magnetic_Field,strlen(Close_North_Magnetic_Field))==0)		return(6); 
	else if(strncmp(Data1,Open_South_Magnetic_Field,strlen(Open_South_Magnetic_Field))==0)			return(7); 
	else if(strncmp(Data1,Close_South_Magnetic_Field,strlen(Close_South_Magnetic_Field))==0)		return(8); 
	else if(strncmp(Data1,Magnetic_Field_Up,strlen(Magnetic_Field_Up))==0)							return(9); 
	else if(strncmp(Data1,Magnetic_Field_Down,strlen(Magnetic_Field_Down))==0)						return(10); 
	else if(strncmp(Data1,Set_Coil_Current,strlen(Set_Coil_Current))==0)							return(11);  
	else if(strncmp(Data1,Set_Current_Offset,strlen(Set_Current_Offset))==0)						return(12);						 	
	else if(strncmp(Data1,Set_Fixture_Ratio,strlen(Set_Fixture_Ratio))==0)							return(13);					 
	else if(strncmp(Data1,Calculate_Fixture_Ratio,strlen(Calculate_Fixture_Ratio))==0)				return(14);														
	else if(strncmp(Data1,Read_Data,strlen(Read_Data))==0)											return(15);
	else if(strncmp(Data1,Read_Coil_Current,strlen(Read_Coil_Current))==0)							return(16);
	else if(strncmp(Data1,Read,strlen(Read))==0)													return(17);
	else if(strncmp(Data1,Sample_1,strlen(Sample_1))==0)											return(18);
	else if(strncmp(Data1,Sample,strlen(Sample))==0)												return(19);
	else if(strncmp(Data1,Sample_magnetic,strlen(Sample_magnetic))==0)								return(20);
	else if(strncmp(Data1,Read_Compass_data,strlen(Read_Compass_data))==0)							return(21);
	else  																							return(200);
}
//========================================================================================

void ProgCommGo(uchar i)
{
	ulong temp,temp2;
	uchar k;
	uint j,f,g,l,Tem_AK8963;
	uint Tem_Current;
	uint a,b,X,d,e;
	switch(i)
	{
		case 1:	//=============================================================Information
			Transmit_arraydata("Fe - Proto2 - QT2 - V2.1");
			Transmit_arraydata("HW Support: V1.0");
			Transmit_arraydata("Protocol: V2.1");
			Transmit_arraydata("Vendor: Intelligent");
			Transmit_arraydata("Date: 2016-12-20");
			Transmit_arraydata("SN: 00001");
			break;
		case 2:	//====================================================================Help
			ProgHelp();
			break;
		case 3://=============================================================reset fixture
			
			Transmit_arraydata("Reset fixture Pass");
			break;
		case 4:		//==========================================================Code_VER
			Transmit_arraydata("Code VER.=1.0V");
			
			break;	
		case 5:	//===============================================================Open_North_Magnetic_Field
			Magnetic_Field('N',1);
			AK8963_Data_Read_XYZ();
			Transmit_arraydata("Open North Magnetic Field Fail");
			break;		
		case 6:	//=================================================================Close_North_Magnetic_Field
			Magnetic_Field('N',1);
			Transmit_arraydata("Close North Magnetic Field Pass");
			break;		
		case 7:	//================================================================Open_South_Magnetic_Field
			Magnetic_Field('S',1);
			AK8963_Data_Read_XYZ();
			Transmit_arraydata("Open Sorth Magnetic Field Pass");	
			break;		
		case 8:	//==================================================================Close_South_Magnetic_Field
			Magnetic_Field('S',0);
			Transmit_arraydata("Close South Magnetic Pass");
			break;		
		case 9://===================================================================Magnetic_Field_Up
			Tem_Current = Read_Current_Value();
			Tem_Current = ++Tem_Current; 
			if(Tem_Current > 1000)
				Tem_Current = 1000;
//			 Cotrol_Coil_Current(Tem_Current);
		     Transmit_arraydata("Set Magnetic Field Pass,Data is:");
			 SendData2(Tem_Current);
			 break;
	    case 10://===================================================================Magnetic_Field_Down
		    Tem_Current = Read_Current_Value();
			Tem_Current = --Tem_Current; 
			if(Tem_Current > 1000)
				Tem_Current = 1000;
//			 Cotrol_Coil_Current(Tem_Current);
		     Transmit_arraydata("Set Magnetic Field Pass,Data is:");
			 SendData2(Tem_Current);
		    break;
		case 11:	//=============================================================Set Coil Current
			Tem_Valu = getNunb(CmdBuffer);
			//Cotrol_Coil_Current(Tem_Valu);
			Transmit_arraydata("Set Coil Current OK, Data is ");
			SendData2(Tem_Valu);
			break;
		case 12:	//=============================================================Set Current Offset
			Tem_Valu = getNunb(CmdBuffer);
			//EEPROM_Write_Uint(Current_Offset,Tem_Valu);
			Transmit_arraydata("Set Current Offset OK, Data is ");
			SendData2(Tem_Valu);
			break;
		case 13:	//=======================================================Set Fixture Ratio
			Tem_Valu = getNunb(CmdBuffer);
			EEPROM_write(0x05,(Tem_Valu));		//fixture ratio gs
			EEPROM_write(0x04,(Tem_Valu>>8));
			Transmit_arraydata("Set Fixture Ratio OK, Data is ");
			SendData2(Tem_Valu);
			break;
		case 14:	//=======================================Calculate Fixture Ratio	
			j = EEPROM_read(0x04);
			j = j<<8;
			j |= EEPROM_read(0x05);			//fixture ratio gs
			Magnetic_Field('N',1);
			Tem_Valu = ReadADCData(30);
			Tem_Valu = j/Tem_Valu;
			EEPROM_write(0x07,(Tem_Valu));		//fixture ratio 
			EEPROM_write(0x06,(Tem_Valu>>8));
			Transmit_arraydata("Calculate Fixture Ratio OK, Data is ");
			SendData2(Tem_Valu);
			Magnetic_Field('N',0);
			break;
		case 15:	//=======================================Read Data
			j = EEPROM_read(0x00);
			j = j<<8;
			j |= EEPROM_read(0x01);		//current data
			Transmit_arraydata("Data is: ");
			SendData2(j);
			break;
		case 16:	//=======================================Read Coil Current
			Magnetic_Field('N',1);
			Tem_Valu = ReadADCData(30);
			Transmit_arraydata("Current is: ");
			SendData2(Tem_Valu);
			Transmit_arraydata("mA");
			Magnetic_Field('N',0);
			break;
		case 17:	//=======================================Read
			j = EEPROM_read(0x06);
			j = j<<8;
			j |= EEPROM_read(0x07);		//fixture ratio
			Transmit_arraydata("mA");
			Magnetic_Field('N',0);
			Calculate_Magnetic_Ratio(j);
			
			
			Transmit_arraydata("magnetic_ratio: ");
			SendData2(Tem_Valu);
			Transmit_arraydata("magnetic_offset: ");
			SendData2(Tem_Valu);
			Transmit_arraydata("current_offset: ");
			SendData2(Tem_Valu);
			Transmit_arraydata("fixture_ratio: ");
			SendData2(Tem_Valu);
			
			AK8963_Data_Processing();
			Transmit_arraydata("AD_sample[0]:");
			SendData2(X_Dat);
			Transmit_arraydata("AD_sample[1]:");
			SendData2(Y_Dat);
			Transmit_arraydata("AD_sample[]:");
			SendData2(Z_Dat);
			
			Print_XYZ_NMS();
			break;
		case 18:	//=======================================Sample1	
			AK8963_Data_Processing();
			Print_XYZ_NMS();
			break;
		case 19:	//=======================================Sample	
			Tem_Valu = getNunb(CmdBuffer);
			Transmit_arraydata("Sample");
			SendData2(Tem_Valu);
			Transmit_arraydata("Pass");
			Transmit_arraydata("Value is:");
			SendData2(Tem_Valu);
			break;
		case 20:	//=======================================Sample magnetic
			Magnetic_Field('N',1);
			Tem_Valu = AK8963_Data_Processing();
			Tem_Valu = (Tem_Valu*magnetic_ratio + magnetic_offset);
			Transmit_arraydata("Sample magnetic pass");
			break;
		case 21:	//=======================================Read Compass data
			Magnetic_Field('N',1);
			Tem_Valu = AK8963_Data_Processing();
			Tem_Valu = (Tem_Valu*magnetic_ratio + magnetic_offset);
			Transmit_arraydata("Sample magnetic pass");
			SendData2(Tem_Valu);
			break;

		case 200:		
			 Transmit_arraydata(error);
			 break;	
			
	}	
	
	for(j=0;j<(cmd_index+2);j++)
		{
			CmdBuffer[j] = '\0';
		}	
}

//===============================================================================

void ProgComm(void)
{

	uchar i=0,j=0,k;
	if(receiveover==1)//接收完成
	{  
		Cont = 0;
	   	if(cmd_index == 0)
		{
		  Transmit_arraydata("@_@");
		  receiveover = 0;
		  cmd_index = 0;
		  return;
		}
		for(i=0;i<35;i++)
		{
			FID_Buffer[i] = CmdBuffer[i];
		}
		process(CmdBuffer);
		i=DataCMP();
		delayms(1);
	
		ProgCommGo(i);		//动作处理
		ProgResponseEnd();	
		receiveover = 0;
		cmd_index = 0; 
	}
	
}


//=============================QT_RESET_FUN=====================================

//==================================================================================
unsigned short getNunb(char *Point)
{
	char *P1 = Point;
	char P2[5] = {0};
	unsigned short A=0;
	signed char n = 0;
	unsigned short i = 1;

	while(*P1!='\0')
	{
		if((*P1>='0')&&(*P1<='9'))
		{
			P2[n++] = *P1 - '0';
		}
		P1++;
	}
	P1 = P2+n-1;
	for(; n>0; n--)
	{
		A += *P1*i;
		i = i*10;
		P1--;
	}
	return A;
}
/*
void Key_Pro(void)
{
	int temp;
	if(NorthKey == 0)
	{
		delayms(10);
		if(NorthKey == 0)
		{
			Tem_Valu = 819;
			ad5660_wr_buf[2]=0;
			ad5660_wr_buf[1]=Tem_Valu>>8;
			ad5660_wr_buf[0]=Tem_Valu;
			Wr_AD5660(ad5660_wr_buf,3);
			Wr_AD5660(ad5660_wr_buf,3);
			Flg_South = 1;
			Flg_North = 0;
		 	Clr_South_Magnetic;
			//delayms(10);
			delayms(100);
			Set_North_Magnetic;	
			delayms(200);	
			Set_North_Magnetic;	
			//delayms(20);
			delayms(100);
			Cont = 0;
			Tem_Valu = Valu_Read();
			Tem_Valu = 6553+(19660*Tem_Valu)/15000;//819==1V 1638==2V
			ad5660_wr_buf[2]=0;
			ad5660_wr_buf[1]=Tem_Valu>>8;
			ad5660_wr_buf[0]=Tem_Valu;
			Wr_AD5660(ad5660_wr_buf,3);
			Wr_AD5660(ad5660_wr_buf,3);
			Flg_KeyOneTime_S = 0;
			Flg_KeyOneTime_N ++;
			if(Flg_KeyOneTime_N > 1)
			{
				Flg_KeyOneTime_N = 0;
				Clr_South_Magnetic;
				Clr_North_Magnetic;
			}
			Transmit_arraydata("ok1\r\n");	
		}
	}
	else if(SouthKey == 0)
	{
		delayms(10);
		if(SouthKey == 0)
		{
			Tem_Valu = 819;
			ad5660_wr_buf[2]=0;
			ad5660_wr_buf[1]=Tem_Valu>>8;
			ad5660_wr_buf[0]=Tem_Valu;
			Wr_AD5660(ad5660_wr_buf,3);
			Wr_AD5660(ad5660_wr_buf,3);
			Flg_South = 0;
			Flg_North = 1;
			Set_North_Magnetic;
			Set_South_Magnetic;		
			delayms(100);
			
			Set_North_Magnetic;
			Set_South_Magnetic;
			delayms(100);	
		    Set_North_Magnetic;
			delayms(500);
			Set_South_Magnetic;
			Cont = 0;
			Tem_Valu = Valu_Read();
			Tem_Valu = 6553+(19660*Tem_Valu)/15000;//819==1V 1638==2V
			ad5660_wr_buf[2]=0;
			ad5660_wr_buf[1]=Tem_Valu>>8;
			ad5660_wr_buf[0]=Tem_Valu;
			Wr_AD5660(ad5660_wr_buf,3);
			Wr_AD5660(ad5660_wr_buf,3);
			Flg_KeyOneTime_N = 0;
			Flg_KeyOneTime_S ++;
			if(Flg_KeyOneTime_S > 1)
			{
				Flg_KeyOneTime_S = 0;
				Clr_South_Magnetic;
				Clr_North_Magnetic;
			}
			Transmit_arraydata("ok2\r\n");	
		}
	}
	else if(UPKey == 0)
	{
		delayms(10);
		if(UPKey == 0)
		{
		    delayms(10);
			Tem_Valu = Valu_Read();
			if(Tem_Valu<=14900) Tem_Valu = Tem_Valu + 10;
			Valu_Save(Tem_Valu);
			Cont = 0;
			delayms(100);
			//while(UPKey == 0);
		}
	}
	else if(DownKey == 0)
	{
		delayms(10);
		if(DownKey == 0)
		{
		    delayms(10);
			Tem_Valu = Valu_Read();
			if(Tem_Valu>=100) Tem_Valu = Tem_Valu - 10;
			Valu_Save(Tem_Valu);
			Cont = 0;
			delayms(100);
			//while(DownKey == 0);
		}
	}
	else if((DownKey == 0)&&(UPKey == 0))
	{
		delayms(100);
		if((DownKey == 0)&&(UPKey == 0))
		{
			temp = EEPROM_read(0x00);
			temp = temp<<8;
			temp |= EEPROM_read(0x01);
			temp = temp - 8000;
			EEPROM_write(0x05,temp);
			EEPROM_write(0x06,(temp>>8));
		
			temp = EEPROM_read(0x03);
			temp = temp<<8;
			temp |= EEPROM_read(0x02);
			temp = temp - 8000;
			EEPROM_write(0x07,temp);
			EEPROM_write(0x08,(temp>>8));		
			
			SendData2(Valu_Read_Data());
			Transmit_arraydata(" Pass");
		}
	}
}

*/
//================================================================================
/*
void AD5160_write(uchar data)
{
    uchar i;
	CS1_H;
    SCL_H;
    SDA_H;
    NOP();
    NOP();
	CS1_L;
	NOP();
	NOP();
    for (i = 0; i < 8; i++)
    {
		NOP();
        if (data & 0x80)
            SDA_H;
        else
            SDA_L;
        SCL_L;
        NOP();
        SCL_H;
        data = data << 1;
    }
    NOP();
	CS1_H;
    SCL_L;
    NOP();
}
*/
//==========================================================================
void LED_Flash(void)
{
	LED_Time ++;
	if(LED_Time > 5000)
	{
		LED_Time = 0;
		if(LED_Time_Flg == 0)
		{
			LED_Time_Flg = 1;
			SetLED_Braod;
		}
		else
		{
			LED_Time_Flg = 0;
			ClrLED_Braod;
		}
	}	
}

void main(void)//=====================================================================================================main
{
//	ulong g;
//	uchar f;
//	uint e;
//	unsigned short kk;
	Write_AD5660(5);
	init_devices();
//	Init_AD5629();
//	ADC_init();
	ad5660_wr_buf[2]=0;
	ad5660_wr_buf[1]=0xFF;
	ad5660_wr_buf[0]=0xFF;
	AK8963_init();
//	Wr_AD5660(ad5660_wr_buf,3);
	while(1)
	{
		LED_Flash();
		ProgComm();
//		Key_Pro();
	}
}	






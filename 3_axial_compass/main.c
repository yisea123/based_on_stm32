// Target : M64
// Crystal: 11.0592M
// change time 2014-07-11


#include "main.h"
#include"Commands.H"


uchar write_buff[4];
uchar read_buff[4];

long 	Tem1_long,Tem2_long;
uchar	SampleSave;
uchar	LED_Time_Flg;
uchar	step = 0;
ulong 	Tem_Valu;
unsigned int	LED_Time;
uchar FID_Buffer[35];
unsigned long Cont;
float Tem1_float,Tem2_float,Tem3_float;



void SendData(unsigned char dat);
unsigned short getNunb(char *Point);
void Valu_Save(ulong i);
uint Valu_Read(void);
uchar TableTest[4];

float South_Float,North_Float;



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
	DDRA &= ~(1<<1)&~(1<<2);
	PORTA |= (1<<1)|(1<<2);
	DDRG &= ~(1<<1)&~(1<<0)&~(1<<2);
	DDRC &= ~(1<<1)&~(1<<0);
	PORTG |= (1<<1)|(1<<0)|(1<<2);
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
//	ADS8320_init();
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
	while(EECR & (1<<EEWE));
	EEAR = address;
	EEDR = data;
	EECR |= (1<<EEMWE);
	EECR = (1<<EEWE) | (1<<EEMWE);
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
void Write_AD5660(unsigned int data)
{
	unsigned char i;
	SCLK2_H;
	NOP();NOP();NOP();NOP();NOP();NOP();NOP()
	NOP();NOP();NOP();NOP();NOP();NOP();NOP();
	DIN2_H;
    SYNC2_H;
	NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP()
	NOP();NOP();NOP();NOP();NOP();NOP();NOP();
	SYNC2_L;
	NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP()
	NOP();NOP();NOP();NOP();NOP();NOP();NOP();
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
}
//=============================================================================
void ProgResponseEnd(void)
{
	USART_Transmit_byte(0x0d); 
	USART_Transmit_byte(0x0a);
	Transmit_arraydata(SY);		
}
//==================================================================================
char StrnCompare(char *str1, const char *str2,char n)
{
    while(n--)
    {
        if((*str1>='A' && *str1<='Z') && (*str2>='a' && *str2<='z'))        //如果str1是大写字母，str2是小写字母
        {
			if(*str1+32==*str2)    //如果str1和str2对应的字母相同，则继续比较下一个
        	{
            	str1++;
            	str2++;
        	}
        	else return 1;
		}
        else if((*str1>='a' && *str1<='z') && (*str2>='A' && *str2<='Z'))   //如果str1是小写字母，str2是大写字母
        {
			if(*str1-32==*str2)    //如果str1和str2对应的字母相同，则继续比较下一个
        	{
            	str1++;
            	str2++;
        	}
        	else return 1;
		}

        else
        {
        if(*str1==*str2)    //如果str1和str2对应的字母相同，则继续比较下一个
        {
            str1++;
            str2++;
        }
        else return 1;
        }
    }
    return 0;
}
//==============================================================================================
uchar DataCMP(void)
{

	uchar * Data1;
	uchar T_B;
	Data1 = CmdBuffer;		
	
				
	if(StrnCompare(Data1,Fixture_Information,19)==0)  							return(1);										 			
	else if(StrnCompare(Data1,Help,4)==0)										return(2); 	
	else if(StrnCompare(Data1,Reset_Fixture,13)==0)								return(3); 
	else if(StrnCompare(Data1,Code_VER,9)==0)									return(4); 
	else if(StrnCompare(Data1,Open_North_Magnetic_Field,  25)==0)				return(5); 
	else if(StrnCompare(Data1,Close_North_Magnetic_Field,26)==0)				return(6); 
	else if(StrnCompare(Data1,Open_South_Magnetic_Field,25)==0)					return(7); 
	else if(StrnCompare(Data1,Close_South_Magnetic_Field,26)==0)				return(8); 
	else if(StrnCompare(Data1,Magnetic_Field_Up,17)==0)							return(9); 
	else if(StrnCompare(Data1,Magnetic_Field_Down,19)==0)						return(10); 
	else if(StrnCompare(Data1,Set_Coil_Current,16)==0)							return(11);  
	else if(StrnCompare(Data1,Set_Current_Offset,18)==0)						return(12);						 	
	else if(StrnCompare(Data1,Set_Fixture_Ratio,17)==0)							return(13);					 
	else if(StrnCompare(Data1,Calculate_Fixture_Ratio,23)==0)					return(14);														
	else if(StrnCompare(Data1,Read_Data,9)==0)									return(15);
	else if(StrnCompare(Data1,Read_Coil_Current,17)==0)							return(16);
	else if(StrnCompare(Data1,Read,cmd_index)==0)								return(17);
	else if(StrnCompare(Data1,Sample_1,cmd_index)==0)							return(18);
	else if(StrnCompare(Data1,Sample_magnetic,15)==0)							return(20);
	else if(StrnCompare(Data1,Sample,6)==0)										return(19);
	else if(StrnCompare(Data1,Read_Compass_data,17)==0)							return(21);
	else if(StrnCompare(Data1,Set_A,5)==0)										return(22);
	else if(StrnCompare(Data1,Set_B,5)==0)										return(23);
	else if(StrnCompare(Data1,Set_Info,8)==0)									return(25);
	else if(StrnCompare(Data1,Set_Date,8)==0)									return(26);
	else if(StrnCompare(Data1,Set_DA,6)==0)										return(24);
	else if(StrnCompare(Data1,Set_SN,6)==0)										return(27);
	
	else  																		return(200);
}
//========================================================================================

void ProgCommGo(uchar i)
{
	uchar TemBuffer[5];
	uchar j;
//	double d;
	int Tem1_Int;
	long AD_Tem2;
	float Tem1_Float,Tem2_Float;
	double Tem1_double,Tem2_double,Tem3_double;
	
	long A,B;
	uchar *p;
	
	unsigned char *r,*w;
	
	switch(i)
	{
		case 1:	//=============================================================Information
			printf("Fe - Proto2 - QT2 - V2.1\r\n");
			//rd24c(TemBuffer,Addr_Info,22);
			printf("%s\r\n",TemBuffer);
			printf("HW Support: V1.0\r\n");
			printf("Protocol: V2.1\r\n");
			printf("Vendor: Intelligent\r\n");
			//rd24c(TemBuffer,Addr_Date,10);
			//printf("Date:%s\r\n",TemBuffer);
			//rd24c(TemBuffer,Addr_SN,6);
			//printf("SN:%s\r\n",TemBuffer);
			printf("Date: 2016-12-20\r\n");
			printf("SN: 00001\r\n");
			break;
		case 2:	//====================================================================Help
			ProgHelp();
			break;
		case 3://=============================================================reset fixture
			Flg_Updata = 1;
			Magnetic_Field('S',0);
			ClrLED_R;
			ClrLED_G;
			printf("Reset fixture Pass");
			break;
		case 4:		//==========================================================Code_VER
			printf("Code VER.=1.0V");
			break;	
		case 5:	//==================================================Open_North_Magnetic_Field
			Magnetic_Field('N',1);
			delayms(100);
			AK8963_Data_Processing();
			printf("Open North Magnetic Field Pass\r\n");
			printf("X axis compass:%d\r\n",Ex_X_Dat);
			printf("Y axis compass:%d\r\n",Ex_Y_Dat);
			printf("Z axis compass:%d\r\n",Ex_Z_Dat);
			break;		
		case 6:	//==================================================Close_North_Magnetic_Field
			Magnetic_Field('N',0);
			
			printf("Close North Magnetic Field Pass");
			break;		
		case 7:	//===============================================Open_South_Magnetic_Field
			Magnetic_Field('S',1);
			delayms(100);
			AK8963_Data_Processing();
			
			printf("Open South Magnetic Field Pass\r\n");
			printf("X axis compass:%d\r\n",Ex_X_Dat);
			printf("Y axis compass:%d\r\n",Ex_Y_Dat);
			printf("Z axis compass:%d\r\n",Ex_Z_Dat);
			break;		
		case 8:	//===============================================Close_South_Magnetic_Field
			Magnetic_Field('S',0);
			printf("Close South Magnetic Pass");
			break;		
		case 9://=======================================================Magnetic_Field_Up
			Flg_Updata = 1;
			Tem1_float = Float_EEPROM_Read(Addr_Current);
			Tem1_float = Tem1_float + 0.5; 
			if(Tem1_float > 1000)
				Tem1_float = 1000;
			Cotrol_Coil_Current(Tem1_float,1);
			printf("Set Magnetic Field Pass,Data is:%3.1f",Tem1_float);
			break;
	    case 10://=====================================================Magnetic_Field_Down
			Flg_Updata = 1;
			Tem1_float = Float_EEPROM_Read(Addr_Current);
			Tem1_float = Tem1_float - 0.5; 
			if(Tem1_float > 1000)
				Tem1_float = 1000;
			Cotrol_Coil_Current(Tem1_float,1);
		    printf("Set Magnetic Field Pass,Data is:%3.1f",Tem1_float);
		    break;
		case 11:	//=============================================================Set Coil Current
			Flg_Updata = 1;
			for(j=0;j<6;j++)
			{
				TemBuffer[j] = CmdBuffer[17+j];
			}
			p = TemBuffer;
			Tem1_float = atof(p);
			//Tem1_float = getNunb(CmdBuffer);
			Cotrol_Coil_Current(Tem1_float,1);
			printf("Set Coil Current OK, Data is %3.1f",Tem1_float);
			break;
		case 12:	//=====================================Set Current Offset
			for(j=0;j<6;j++)
			{
				TemBuffer[j] = CmdBuffer[19+j];
			}
			p = TemBuffer;
			Tem1_float = atof(p);
			printf("Set Current Offset OK, Data is %3.2f\n", Tem1_float);
			Tem2_float = Float_EEPROM_Read(AddrF_CurrOff);
			Tem1_float = Tem1_float + Tem2_float;
			Float_EEPROM_write(AddrF_CurrOff,Tem1_float);
			Flg_Updata = 1;
			Tem1_float = Float_EEPROM_Read(Addr_Current);
			Cotrol_Coil_Current(Tem1_float,1);
			
			break;
		case 13:	//==========================================Set Fixture Ratio
			Tem_Valu = getNunb(CmdBuffer);
			EEPROM_Write_Uint(Addr_Ratio_Gs,Tem_Valu);
			printf("Set Fixture Ratio OK, Data is %d\r\n",Tem_Valu);
			break;
		case 14:	//=======================================Calculate Fixture Ratio	
			Tem1_float = 3000;
			Tem2_float = Float_EEPROM_Read(Addr_Current);
			Tem2_float = (Tem1_float/Tem2_float);//fixture_ration = 3000/i
			Float_24C02_write(AddrF_FixRat,Tem2_float);
			printf("Calculate Fixture Ratio OK, Data is %3.2f\r\n",Tem2_float);
			break;
		case 15:	//=======================================Read Data
			Tem1_float = Float_EEPROM_Read(Addr_Current);
			printf("Data is:%3.1f\r\n",Tem1_float);
			break;
		case 16:	//=======================================Read Coil Current
			Tem1_float = Read_Current();
			printf("Current is:%3.2fmA\r\n", Tem1_float);
			break;
		case 17:	//=======================================Read
			Float_24C02_Read(AddrF_A);
			Tem1_float = Float_24C02_Read(AddrF_A);
			Tem2_float = Float_24C02_Read(AddrF_B);
			printf("magnetic_ratio:%3.2f\r\n",Tem1_float);
			printf("magnetic_offset:%3.2f\r\n",Tem2_float);
			
			Tem1_float = Float_EEPROM_Read(AddrF_CurrOff);
			printf("current_offset:%3.2f\r\n",Tem1_float);
			
			Tem1_float = Float_24C02_Read(AddrF_FixRat);
			printf("fixture_ratio:%3.2f\r\n",Tem1_float);
			
			AD_sample_X = E24C02_Read_Uint(Currer_I1);
			AD_sample_Y = E24C02_Read_Uint(Currer_I2);
			AD_sample_Z = E24C02_Read_Uint(Currer_I3);
			printf("AD_sample[0]:%d\r\n",AD_sample_X);
			printf("AD_sample[1]:%d\r\n",AD_sample_Y);
			printf("AD_sample[2]:%d\r\n",AD_sample_Z);
			
			//AK8963_Data_Processing();
			NMS_sample_X = 1000; 
			NMS_sample_Y = 3000;
			NMS_sample_Z = 3500;
			printf("NMS_sample[0]:%d\r\n",NMS_sample_X);
			printf("NMS_sample[1]:%d\r\n",NMS_sample_Y);
			printf("NMS_sample[2]:%d\r\n",NMS_sample_Z);
			break;
		case 18:	//=======================================Sample1	
			AK8963_Data_Processing();
			printf("X axis compass:%d\r\n",Ex_X_Dat);
			printf("Y axis compass:%d\r\n",Ex_Y_Dat);
			printf("Z axis compass:%d\r\n",Ex_Z_Dat);
			break;
		case 19:	//=======================================Sample	
			Tem_Valu = getNunb(CmdBuffer);
			switch(SampleSave)
			{
				case 0:
					E24C02_Write_Uint(Sample1000,Tem_Valu);
					SampleSave ++;
					break;
				case 1:
					E24C02_Write_Uint(Sample3000,Tem_Valu);
					SampleSave ++;
					break;
				case 2:
					E24C02_Write_Uint(Sample3500,Tem_Valu);
					SampleSave = 0;
					break;		
			}
			printf("Sample %d Pass",Tem_Valu);
			printf(" Value is: %d",Tem_Valu);
			break;
		case 20:	//=======================================Sample magnetic
			Calculate_Sample_magnetic();
			printf("Sample magnetic pass");
			break;
		case 21:	//=======================================Read Compass data
			Tem1_float = Float_24C02_Read(AddrF_A);
			Tem2_float = Float_24C02_Read(AddrF_B);
			
			Tem3_float = Read_NMS(0);
//			NMS_sample_Y = Tem3_float; 
			South_X = 0;
			South_Y = 0;
			South_Z = 0;
			North_X = 0;
			North_Y = 0;
			North_Z = 0;
			Tem1_float = Tem1_float*Tem3_float;
			Tem1_long = Tem1_float + Tem2_float;
			//printf("Current %d\r\n",Tem1_long);
			if((Tem1_long < 3060)&&(Tem1_long > 2940))
			{
				SetLED_G;
				ClrLED_R;
			}
			else
			{
				ClrLED_G;
				SetLED_R;
			}
			if(Tem1_long < 200)	
			{
				Tem1_long = 0;
			}
			printf("Current compass is %d\r\n",Tem1_long);
			break;
		case 22:	//=======================================set a
			for(j=0;j<6;j++)
			{
				TemBuffer[j] = CmdBuffer[6+j];
			}
			p = TemBuffer;
			Tem1_float = atof(p);
			printf("Set A %3.2f", Tem1_float);
			Float_24C02_write(AddrF_A,Tem1_float);
			break;
		case 23:	//=======================================set b
			for(j=0;j<6;j++)
			{
				TemBuffer[j] = CmdBuffer[6+j];
			}
			p = TemBuffer;
			Tem1_float = atof(p);
			printf("Set B %3.2f", Tem1_float);
			Float_24C02_write(AddrF_B,Tem1_float);
			break;	
		case 24:	//=======================================set da
			Flg_Updata = 1;
			for(j=0;j<6;j++)
			{
				TemBuffer[j] = CmdBuffer[7+j];
			}
			p = TemBuffer;
			Tem1_float = atof(p);
			printf("Set DA %3.2f", Tem1_float);
			Float_EEPROM_write(AddrF_DAOff,Tem1_float);
				
			break;	
		case 25:	//=======================================set info
			for(j=0;j<22;j++)
			{
				TemBuffer[j] = CmdBuffer[9+j];
			}
			wt24c(TemBuffer,Addr_Info,22);
			rd24c(TemBuffer,Addr_Info,22);
			printf("24c02 %s\r\n",TemBuffer);
			break;	
		case 26:	//=======================================set date
			for(j=0;j<10;j++)
			{
				TemBuffer[j] = CmdBuffer[9+j];
			}
			wt24c(TemBuffer,Addr_Date,10);
			rd24c(TemBuffer,Addr_Date,10);
			printf("24c02 %s\r\n",TemBuffer);
			break;	
		case 27:	//=======================================set sn
			for(j=0;j<6;j++)
			{
				TemBuffer[j] = CmdBuffer[7+j];
			}
			wt24c(TemBuffer,Addr_SN,6);
			rd24c(TemBuffer,Addr_SN,6);
			printf("24c02 %s\r\n",TemBuffer);
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
	if(receiveover==1)//鎺ユ敹瀹屾垚
	{  
		Cont = 0;
	   	if(cmd_index == 0)
		{
		  Transmit_arraydata("@_@");
		  receiveover = 0;
		  cmd_index = 0;
		  return;
		}

//		process(CmdBuffer);
		i=DataCMP();
		delayms(1);
		ProgCommGo(i);		//鍔ㄤ綔澶勭悊
		ProgResponseEnd();	
		receiveover = 0;
		cmd_index = 0; 
	}
	
}
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
//==================================================================
void Key_Pro(void)
{
	if(NorthKey == 0)
	{
		delayms(10);
		if(NorthKey == 0)
		{
			Magnetic_Field('N',1);
			Transmit_arraydata("north\r\n");
		}
	}	
	else if(SouthKey == 0)
	{
		delayms(10);
		if(SouthKey == 0)
		{
			Magnetic_Field('S',1);
			Transmit_arraydata("south\r\n");
		}
	}	
	else if(UPKey == 0)
	{
		delayms(10);
		if(UPKey == 0)
		{
	//		Key_Control(1);
			Key_Control_Up();
			Transmit_arraydata("up\r\n");
		}
	}	
	else if(DownKey == 0)
	{
		delayms(10);
		if(DownKey == 0)
		{
			Key_Control_Down();
			Transmit_arraydata("down\r\n");
		}		
	}	
	if(UPKey != 0)
	{
		Step_Key_Up = 0;
	}
	if(DownKey != 0)
	{
		Step_Key_Down = 0;
	}
}


void main(void)//=====================================================================================================main
{
	delayms(100);
	init_devices();
	TWI_init();
	EEPROM_Init();
//	ADC_init();
	AK8963_init();
	ads1110Config();
	while(1)
	{
		LED_Flash();
		ProgComm();
		Key_Pro();
		//printf("float is:%d\n",125.32);
	}
}	






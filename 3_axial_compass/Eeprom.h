#ifndef __eeprom_h__
#define __eeprom_h__

//eeprom
#define Addr_Current			0x00
#define Addr_Ratio_Gs			0x06

//24C02
#define AddrF_DAOff				0x04
#define Sample1000				0x08
#define Sample3000				0x0a
#define Sample3500				0x0c

#define Currer_I1				0x20
#define Currer_I2				0x22
#define Currer_I3				0x24

//#define Addr_Current_Offset 	0x06
#define AddrF_CurrOff			0x10
#define AddrF_FixRat			0x14
#define AddrF_A					0x18
#define AddrF_B					0x1c

#define Addr_Info				0x40
#define Addr_Date				0x56
#define Addr_SN					0x60


#define Addr_First_Time			0x30
extern uchar x[4];

extern void EEPROM_Init(void);
extern void FtoC(float a);
extern float CtoF(void);

extern void EEPROM_Write_Uint(uchar addr,uint valu);
extern uint EEPROM_Read_Uint(uchar addr);
extern void Float_EEPROM_write(uchar Addr,float a);
extern float Float_EEPROM_Read(uchar Addr);

extern void E24C02_Write_Uint(uint addr,uint valu);
extern uint E24C02_Read_Uint(uint addr);
extern void Float_24C02_write(uint addr,float a);
extern float Float_24C02_Read(uint addr);


//extern void Float_E24C02_write(uchar Addr,float a);
//extern float Float_E24C02_Read(uchar Addr);

#endif
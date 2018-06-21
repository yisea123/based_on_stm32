#ifndef __24CXX_H
#define __24CXX_H

//////////////////////////////////////////////////////////////////////////////////
#define AT24C01     127
#define AT24C02     255
#define AT24C04     511
#define AT24C08     1023
#define AT24C16     2047
#define AT24C32     4095
#define AT24C64     8191
#define AT24C128    16383
#define AT24C256    32767
#define AT24C512B       65535
#define EE_TYPE AT24C512B
extern u16 AT24CXX_ReadTwoByte(u16 ReadAddr);
extern void AT24CXX_WriteTwoByte(u16 WriteAddr,u16 DataToWrite);
extern u8 AT24CXX_ReadOneByte(u16 ReadAddr);                            //ָ����ַ��ȡһ���ֽ�
extern void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite);     //ָ����ַд��һ���ֽ�
extern void AT24CXX_WriteOTwoByte(u16 WriteAddr,u16 DataToWrite);
extern void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len);//ָ����ַ��ʼд��ָ�����ȵ�����
extern u32 AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len);                    //ָ����ַ��ʼ��ȡָ����������
extern void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite);    //��ָ����ַ��ʼд��ָ�����ȵ�����
extern void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead);       //��ָ����ַ��ʼ����ָ�����ȵ�����
extern u8 AT24CXX_Check(void);  //�������
extern void AT24CXX_Init(void); //��ʼ��IIC
#endif

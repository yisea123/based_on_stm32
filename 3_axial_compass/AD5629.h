//#include"INIT.H"
#ifndef _AD5629_h
#define _AD5629_h


#define   SDA_out()   	  setbit(DDRD,5)    //����SDAΪ���
#define   SDA_in()    	  clrbit(DDRD,5)    //����SDAΪ����
#define   SDA_Hig()   	  setbit(PORTD,5)   //����SDAΪ�ߵ�ƽ
#define   SDA_Low()   	  clrbit(PORTD,5)   //����SDAΪ�͵�ƽ
#define   SDA_action()    getbit(PIND,5)    //��ȡӦ���ź�

#define   SCL_out()   	  setbit(DDRD,6)    //����SCLΪ���
#define   SCL_Hig()   	  setbit(PORTD,6)   //����SCLΪ�ߵ�ƽ
#define   SCL_Low()   	  clrbit(PORTD,6)   //����SCLΪ�͵�ƽ

#define   CLR_out()   	  setbit(DDRD,4)    //����SDAΪ���
#define   CLR_Hig()   	  setbit(PORTD,4)   //����CLRΪ�ߵ�ƽ
#define   CLR_Low()   	  clrbit(PORTD,4)   //����CLRΪ�ߵ�ƽ

//�����
#define	  COMD1	  0x00  //д������Ĵ���n
#define	  COMD2	  0x10	//����DAC�Ĵ���n
#define	  COMD3	  0x20	//д������Ĵ���n������ȫ��(���LDAC)
#define	  COMD4	  0x30	//д�벢����DACͨ��n
#define	  COMD5	  0x40	//DAC����/�ϵ�Z
#define	  COMD6	  0x50	//�����������Ĵ���
#define	  COMD7	  0x60	//����LDAC�Ĵ���
#define	  COMD8	  0x70	//��λ(�ϵ縴λ)
#define	  COMD9	  0x80	//�����ڲ�REF�Ĵ���
#define	  COMD10  0x90	//ʹ�ܶ��ֽ�ģʽ
//���ͨ������
#define	  DAC_A	  0x00 	  
#define	  DAC_B	  0x01 	  
#define	  DAC_C	  0x02 	  
#define	  DAC_D	  0x03
#define	  DAC_E	  0x04 	  
#define	  DAC_F	  0x05
#define	  DAC_G	  0x06 	  
#define	  DAC_H	  0x07
#define	  DAC_All 0x0f

void Init_AD5629(void);
unsigned short Volt2DAC(unsigned short Voltage);
void Write_AD5629(unsigned char Commad,unsigned char CH, unsigned short dat,unsigned char IS_V);
unsigned char  Read_AD5629(unsigned char Commad,unsigned char CH,unsigned char IS_V);


#endif























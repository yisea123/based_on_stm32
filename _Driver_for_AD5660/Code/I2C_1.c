//#include "GlobalDef.h"
//#define ulong unsigned long
#include "config.h"
//��ʼ��IIC
void IIC_Init(void)
{
    RCC->APB2ENR|=1<<3;//��ʹ������IO PORTBʱ��
    GPIOB->CRL&=0X00FFFFFF;//PB6,7 �������
    GPIOB->CRL|=0X33000000;
    GPIOB->ODR|=3<<6;     //PB6,7 �����
//  GPIO_InitTypeDef  GPIO_InitStructure;
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;//??????
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//100MHz
//  GPIO_Init(GPIOA, &GPIO_InitStructure);//???
//  IIC_SCL=1;
//  IIC_SDA=1;
}
//����IIC��ʼ�ź�
void IIC_Start(void)
{
    SDA_OUT();     //sda�����
    IIC_SDA=1;
    IIC_SCL=1;
    Delayus(50);
    IIC_SDA=0;//START:when CLK is high,DATA change form high to low
    Delayus(60);
    IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ��������
}
//����IICֹͣ�ź�
void IIC_Stop(void)
{
    SDA_OUT();//sda�����
    IIC_SCL=0;
    IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
    Delayus(60);
    IIC_SCL=1;
    IIC_SDA=1;//����I2C���߽����ź�
    Delayus(60);
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
    u8 ucErrTime=0;
    SDA_IN();      //SDA����Ϊ����
    IIC_SDA=1;
    Delayus(10);//ʵ��10us(1);
    IIC_SCL=1;
    Delayus(10);
    while(READ_SDA)
    {
        ucErrTime++;
        if(ucErrTime>250)
        {
            IIC_Stop();
            return 1;
        }
    }
    IIC_SCL=0;//ʱ�����0
    return 0;
}
//����ACKӦ��
void IIC_Ack(void)
{
    IIC_SCL=0;
    SDA_OUT();
    IIC_SDA=0;
    Delayus(20);
    IIC_SCL=1;
    Delayus(50);
    IIC_SCL=0;
}
//������ACKӦ��
void IIC_NAck(void)
{
    IIC_SCL=0;
    SDA_OUT();
    IIC_SDA=1;
    Delayus(20);
    IIC_SCL=1;
    Delayus(50);
    IIC_SCL=0;
}
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��
void IIC_Send_Byte(u8 txd)
{
    u8 t;
    SDA_OUT();
    IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0; t<8; t++)
    {
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1;
        Delayus(20);   //��TEA5767��������ʱ���Ǳ����
        IIC_SCL=1;
        Delayus(20);
        IIC_SCL=0;
        Delayus(20);
    }
}
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK
u8 IIC_Read_Byte(unsigned char ack)
{
    unsigned char i,receive=0;
    SDA_IN();//SDA����Ϊ����
    for(i=0; i<8; i++ )
    {
        IIC_SCL=0;
        Delayus(20);
        IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)
        {
            receive++;
        }
        Delayus(10);
    }
    if (!ack)
    {
        IIC_NAck();    //����nACK
    }
    else
    {
        IIC_Ack();    //����ACK
    }
    return receive;
}
//u8 IIC_Read_One_Byte(u8 Address,u8 ReadAddr)
//{
//      u8  temp;
//
//      IIC_Start();
//      IIC_Send_Byte((Address<<1)&0xfe);                   //����������ַ0XA0,д����
//      IIC_Send_Byte(Address);                   //����������ַ0XA0,д����
//      IIC_Wait_Ack();
//
//    IIC_Send_Byte(ReadAddr);               //���͵͵�ַ
//      IIC_Wait_Ack();
//      IIC_Start();
//      IIC_Send_Byte((Address<<1)|0x01);           //�������ģʽ
//  //  IIC_Send_Byte(Address|0x01);           //�������ģʽ
//      IIC_Wait_Ack();
//      temp=IIC_Read_Byte(0);
//  //  IIC_NAck();
//      IIC_Stop();
//      Delayus(10);                             //��ʱһ��ʱ�䲻Ȼ����д���ɹ�
//      return temp;
//}
u8 IIC_Read_One_Byte(u8 Address,u8 ReadAddr)
{
    u8  temp;
    IIC_Start();
    IIC_Send_Byte((Address<<1)&0xfe);                   //����������ַ0XA0,д����
    IIC_Wait_Ack();
    IIC_Send_Byte(ReadAddr);               //���͵͵�ַ
    IIC_Wait_Ack();
    IIC_Start();
    IIC_Send_Byte((Address<<1)|0x01);           //�������ģʽ
    IIC_Wait_Ack();
    temp=IIC_Read_Byte(0);
    IIC_Stop();
    Delayus(100);                            //��ʱһ��ʱ�䲻Ȼ����д���ɹ�
    return temp;
}
void IIC_Write_One_Byte(u8 daddr,u8 COMM,u8 data)
{
    IIC_Start();
    IIC_Send_Byte((daddr<<1)&0xfe);                   //����������ַ0XA0,д����
    IIC_Wait_Ack();
    IIC_Send_Byte(COMM);               //���͵͵�ַ
    IIC_Wait_Ack();
    IIC_Send_Byte(data);
    IIC_Wait_Ack();
    IIC_Stop();
    Delayus(100);                            //��ʱһ��ʱ�䲻Ȼ����д���ɹ�
}
u16 IIC_Read_TWO_Byte(u8 Address,u8 ReadAddr)
{
    u8  tempH,tempL;
    IIC_Start();
    IIC_Send_Byte((Address<<1)&0xfe);                   //����������ַ0XA0,д����
    IIC_Wait_Ack();
    IIC_Send_Byte(ReadAddr);               //���͵͵�ַ
    IIC_Wait_Ack();
    IIC_Start();
    IIC_Send_Byte((Address<<1)|0x01);           //�������ģʽ
    IIC_Wait_Ack();
    tempL=IIC_Read_Byte(1);
    tempH=IIC_Read_Byte(0);
    IIC_Stop();
    Delayus(100);                            //��ʱһ��ʱ�䲻Ȼ����д���ɹ�
    return (tempH<<8)+tempL;
}
u8 IIC_1_line_Dvice_find(u8 daddr)
{
    IIC_Start();
    IIC_Send_Byte((daddr<<1)&0xfe);                   //����������ַ0XA0,д����
    if(IIC_Wait_Ack()==0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
void IIC_ALS_intit(void)
{
    IIC_Start();
    IIC_Send_Byte(0x52);
    IIC_Wait_Ack();
    IIC_Send_Byte(0x8F); //0x8f
    IIC_Wait_Ack();
    IIC_Send_Byte(0x01);
    IIC_Wait_Ack();
    IIC_Start();
    IIC_Send_Byte(0x52);
    IIC_Wait_Ack();
    IIC_Send_Byte(0x81);
    IIC_Wait_Ack();
    IIC_Send_Byte(0x02);
    IIC_Wait_Ack();
    IIC_Start();
    IIC_Send_Byte(0x52);
    IIC_Wait_Ack();
    IIC_Send_Byte(0x80);
    IIC_Wait_Ack();
    IIC_Send_Byte(0x03);
    IIC_Wait_Ack();
    IIC_Stop();
//   IIC_Start();
//   IIC_Send_Byte(0x52);
//   IIC_Wait_Ack();
//   IIC_Send_Byte(0x80);
//   IIC_Wait_Ack();
//   IIC_Send_Byte(0x01);
//   IIC_Wait_Ack();
//   IIC_Stop();
//
//   Delayms(10);
//
//   IIC_Start();
//   IIC_Send_Byte(0x52);
//   IIC_Wait_Ack();
//   IIC_Send_Byte(0x81);
//   IIC_Wait_Ack();
//   IIC_Send_Byte(0xED);
//   IIC_Wait_Ack();
//   IIC_Stop();
//
//   Delayms(100);
//
//   IIC_Start();
//   IIC_Send_Byte(0x52);
//   IIC_Wait_Ack();
//   IIC_Send_Byte(0x83);
//   IIC_Wait_Ack();
//   IIC_Send_Byte(0x70);
//   IIC_Wait_Ack();
//   IIC_Stop();
//
//   Delayms(10);
//
//   IIC_Start();
//   IIC_Send_Byte(0x52);
//   IIC_Wait_Ack();
//   IIC_Send_Byte(0x84);
//   IIC_Wait_Ack();
//   IIC_Send_Byte(0x17);
//   IIC_Wait_Ack();
//   IIC_Stop();
//
//   Delayms(10);
//
//   IIC_Start();
//   IIC_Send_Byte(0x52);
//   IIC_Wait_Ack();
//   IIC_Send_Byte(0x85);
//   IIC_Wait_Ack();
//   IIC_Send_Byte(0x10);
//   IIC_Wait_Ack();
//   IIC_Stop();
//
//
//   Delayms(10);
//
//   IIC_Start();
//   IIC_Send_Byte(0x52);
//   IIC_Wait_Ack();
//   IIC_Send_Byte(0x86);
//   IIC_Wait_Ack();
//   IIC_Send_Byte(0x27);
//   IIC_Wait_Ack();
//   IIC_Stop();
//
//
//   Delayms(10);
//
//   IIC_Start();
//   IIC_Send_Byte(0x52);
//   IIC_Wait_Ack();
//   IIC_Send_Byte(0x82);
//   IIC_Wait_Ack();
//   IIC_Send_Byte(0x10);
//   IIC_Wait_Ack();
//   IIC_Stop();
//
//   Delayms(10);
//
//   IIC_Start();
//   IIC_Send_Byte(0x52);
//   IIC_Wait_Ack();
//   IIC_Send_Byte(0x87);
//   IIC_Wait_Ack();
//   IIC_Send_Byte(0x01);
//   IIC_Wait_Ack();
//   IIC_Stop();
//
//   Delayms(10);
//
//   IIC_Start();
//   IIC_Send_Byte(0x52);
//   IIC_Wait_Ack();
//   IIC_Send_Byte(0x80);
//   IIC_Wait_Ack();
//   IIC_Send_Byte(0x03);
//   IIC_Wait_Ack();
//   IIC_Stop();
//
//   Delayms(10);
//
//   IIC_Read_One_Byte(0x29,0xB4);//LOW
//   IIC_Read_One_Byte(0x29,0xB5);//HIGT
//   Delayms(200);
//
//    IIC_Start();
//      IIC_Send_Byte(0x52);                   //����������ַ0XA0,д����
//      IIC_Wait_Ack();
//
//    IIC_Send_Byte(0xB4);               //���͵͵�ַ
//      IIC_Wait_Ack();
//      IIC_Start();
//      IIC_Send_Byte(0x53);           //�������ģʽ
//      IIC_Wait_Ack();
//      IIC_Read_Byte(1);
//    IIC_Read_Byte(0);
    /*
       IIC_Read_One_Byte(0x29,0xB4);//LOW
         IIC_Read_One_Byte(0x29,0xB5);//HIGT

         Delayms(500);

       IIC_Read_One_Byte(0x29,0xB4);//LOW
         IIC_Read_One_Byte(0x29,0xB5);//HIGT

         IIC_Start();
         IIC_Send_Byte(0x52);
         IIC_Wait_Ack();
         IIC_Send_Byte(0xE1);
         IIC_Wait_Ack();
         IIC_Stop();
    */
}

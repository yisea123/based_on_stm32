#ifndef _TWI_H_
#define _TWI_H_


#define  TW_START 				 0X08
#define  MT_SLA_ACK  			 0X18
#define  MT_DATA_ACK  			 0X28

#define TestACK 				 (TWSR&0xF8)
#define ACK()					 (TWCR |= (1<<TWEA))
#define NACK()					 (TWCR &= ~(1<<TWEA))


void TWI_init(void);
void TWI_Start(void);
void TWI_Stop(void);
uchar TWI_Write(uchar data);
uchar TWI_Read(void);
uchar i2c_read(uchar ack);

extern void TWI_init(void);
extern void TWI_Start(void);
extern void TWI_Stop(void);
extern uchar TWI_Write(uchar data);
extern uchar TWI_Read(void);
extern uchar i2c_read(uchar ack);




#endif
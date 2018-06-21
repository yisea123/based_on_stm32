//Ä£ÄâIIC
#ifndef _DUT_ADC_H
#define _DUT_ADC_H

#define regStat  0x07
#define READ  1
#define WRITE  0
#define AD_SLAVE_ID 

#define REG_SEQ 4
#define REG_DELAY 7
#define REG_CHMAP0 6
#define REG_CTRL1 1
#define REG_CTRL2 2
#define REG_DATA0 8
#define REG_DATA1 9
#define REG_DATA2 10
#define REG_STAT 0
#define REG_OC_DIRECT 27
#define SEQ_MDREN 2
#define Fs  31.2
#define numSamples 100

extern uint8_t BufRead[4];
extern uint32_t register_read(uint8_t Addr,uint8_t * BufRead,uint8_t Byte_Num);
extern void register_write(uint8_t Addr,uint8_t * wrBuf,uint8_t Byte_Num);
extern void Setup_DUT_AD(void);
extern uint32_t Data_processing(void);
extern void Read_AD_Data(void);
extern void Uart_Decoder(void);
extern void Decoder(uint8_t * string1);
extern uint32_t Complement_ADC(uint32_t data);
extern float data3ToInt(uint8_t *data);
extern void DecoderRead(uint8_t * string1);

#endif


#ifndef _AD5660_h_
#define _AD5660_h_
extern unsigned char ad5660_wr_buf[3];
void AD5660Init(void);
void Wr_AD5660(unsigned char *Buf,unsigned char count);
#endif


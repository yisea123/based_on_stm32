#ifndef __AD5660
#define __AD5660


//#define DIN_5660_0()                  GPIO_ResetBits(GPIOB, GPIO_Pin_6)
//#define DIN_5660_1()                  GPIO_SetBits(GPIOB, GPIO_Pin_6)
//#define SCLK_5660_0()                 GPIO_ResetBits(GPIOB, GPIO_Pin_5)
//#define SCLK_5660_1()                 GPIO_SetBits(GPIOB, GPIO_Pin_5)
//#define SYNC_5660_0()                 GPIO_ResetBits(GPIOB, GPIO_Pin_4)
//#define SYNC_5660_1()                 GPIO_SetBits(GPIOB, GPIO_Pin_4)

//#define SW_5660_0()                 GPIO_ResetBits(GPIOB, GPIO_Pin_7)
//#define SW_5660_1()                 GPIO_SetBits(GPIOB, GPIO_Pin_7)

/*****************************************************************************/
//#define DIN_5660_0()                  GPIO_ResetBits(GPIOB, GPIO_Pin_1)
//#define DIN_5660_1()                  GPIO_SetBits(GPIOB, GPIO_Pin_1)
//#define SCLK_5660_0()                 GPIO_ResetBits(GPIOB, GPIO_Pin_10)
//#define SCLK_5660_1()                 GPIO_SetBits(GPIOB, GPIO_Pin_10)
//#define SYNC_5660_0()                 GPIO_ResetBits(GPIOB, GPIO_Pin_11)
//#define SYNC_5660_1()                 GPIO_SetBits(GPIOB, GPIO_Pin_11)


/*****************************************************************************/
#define DIN_5660_0()                  GPIO_ResetBits(GPIOA, GPIO_Pin_7)
#define DIN_5660_1()                  GPIO_SetBits(GPIOA, GPIO_Pin_7)
#define SCLK_5660_0()                 GPIO_ResetBits(GPIOA, GPIO_Pin_5)
#define SCLK_5660_1()                 GPIO_SetBits(GPIOA, GPIO_Pin_5)
#define SYNC_5660_0()                 GPIO_ResetBits(GPIOA, GPIO_Pin_4)
#define SYNC_5660_1()                 GPIO_SetBits(GPIOA, GPIO_Pin_4)



#define CLR_DIN()		DIN_5660_0()                  
#define	SET_DIN() 		DIN_5660_1()                 
#define	CLR_SCLK()	 	SCLK_5660_0()                
#define	SET_SCLK()		 SCLK_5660_1()                 
#define	CLR_SYNC()		 SYNC_5660_0()                
#define	SET_SYNC()		 SYNC_5660_1()       

extern void AD5660_Out(uint16_t Current);
extern void SPI_AD5660_Init(void);


void Wr_AD5660(unsigned char *Buf,unsigned char count);


#endif

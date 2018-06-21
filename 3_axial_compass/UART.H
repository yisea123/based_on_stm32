/****************************
只有内部用到的全局变量
****************************/
#ifndef __UART_H__
#define __UART_H__

//#define uchar unsigned char  
//#define uint unsigned int    
//#define ulong unsigned long  

#ifdef  UART1_GLOBALS
uint baudselect(unsigned long  baud);//baud的选择
uchar USART_Receive_byte(void);
void uart1_rx_isr(void); //uart has received a character in UDR
#endif

/****************************
外部用到的全局变量
****************************/
#ifdef UART1_GLOBALS
#define UART1_EXT
#else
#define UART1_EXT extern
#endif
  
UART1_EXT uchar *PointReceLen;
UART1_EXT void USART_Transmit_byte(uchar bytedata);

//UART1_EXT uchar ReceBUFF[255];	    
//UART1_EXT uchar SendBuff[255];	     
UART1_EXT uchar receiveover;
UART1_EXT uchar receiveover0;
UART1_EXT uchar cmd_index;
UART1_EXT uchar cmd_index0;
UART1_EXT uchar OldReceLen;
UART1_EXT uchar ReceLenCount;
UART1_EXT uchar ReceLen;
UART1_EXT void uart1_init(ulong baud);
UART1_EXT void uart0_init(ulong baud);
UART1_EXT void USART_Transmit_byte(uchar bytedata);
UART1_EXT void USART_Transmit_byte0(uchar bytedata);
UART1_EXT void USART_Transmit_arraydata(uchar lenght,uchar arraydata[]);
UART1_EXT void USART0_Transmit_arraydata(uchar lenght,uchar arraydata[]);

UART1_EXT void Transmit_arraydataConst(const uchar *point);
UART1_EXT unsigned char string_compare(unsigned char *String1,unsigned char *String2); //字符串对比函数
UART1_EXT void ProgHelp(void);
UART1_EXT void ProgFixture_ID(void);
UART1_EXT void ProgHelp(void);
UART1_EXT void ProgInformation(void);
UART1_EXT uchar Module;
UART1_EXT unsigned int Freq;
UART1_EXT void output(ulong freq_value);
UART1_EXT void init_ad9833(void);
UART1_EXT void SPI_AD5160BRJ100CCSwrite1byte(uchar data);
void Transmit_arraydata(uchar *point);


//====================================函数声明====================================
uchar VCT_Rest_Fix(void);
uchar VCT_Hold_Up(void);
uchar VCT_Hold_Down(void);
uchar VCT_Plug_In(void);
uchar VCT_Plug_Out(void);
uchar VCT_Hold_In(void);
uchar VCT_Hold_Out(void);
uchar VCT_Mic_Close(void);
uchar VCT_Mic_Open(void);
uchar SMT_Qt_Back(void);
uchar SMT_Qt_Up(void);
uchar QT_Reset_Init(void);

void delayms(uint ms);

void HC595_write(uchar data);

//Add by Ryan
#define CMD_MAX_LEN 64						  			   //命令最大长度
#define PROPMPTS    "*_*"
UART1_EXT char CmdBuffer[CMD_MAX_LEN];				  			   //命令缓冲区
UART1_EXT char CmdBuffer0[CMD_MAX_LEN];

#endif


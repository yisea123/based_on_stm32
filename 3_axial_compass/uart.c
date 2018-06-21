//ICC-AVR  2013-10-29
// Target : M64
// Crystal: 11.059Mhz
#define UART1_GLOBALS
#include "main.h"


uchar Module;
uchar Flg_Uart;
uint Freq;
char CmdBuffer[64]={0};	  				 //命令接受缓冲区
char CmdBuffer2[64]={0};	  				 //命令接受缓冲区
char CmdBuffer1[64]={0};	
char CmdBuffer0[64]={0};	
uchar receiveover;

extern int _textmode;
 
int putchar(char c)
{
	if (_textmode && c == '\n')
		putchar('\r');
    /* Wait for empty transmit buffer */
	while ( !(UCSR0A & (1<<UDRE1)) )
        ;                     
    /* Putting data into buffer , sends the data */
	UDR0 = c;  
	return c;
}


//   11.0592M
uint baudselect(unsigned long baud)//baud rate
{
	uint bauddata=0;
	if(baud==9600) bauddata=0x47;       //baud=9600
	else if(baud==19200) bauddata=0x23;  //baud=19200
	else if(baud==57600) bauddata=0x0B;  //baud=57600
	else if(baud==115200) bauddata=0x05;
	return(bauddata);
}

//    7.3728M
/*
uint baudselect(unsigned long  baud)//baud rate
{
	uint bauddata=0;
	if(baud==9600) bauddata=0x2F;       //baud=9600
	else if(baud==19200) bauddata=0x17;  //baud=19200
	else if(baud==57600) bauddata=0x07;  //baud=57600
	else if(baud==115200) bauddata=0x03;
	return(bauddata);
}
*/
void uart1_init(ulong baud)
{
 CLI(); //disable all interrupts
 UCSR0B = 0x00; //disable while setting baud rate
 UCSR0A=0x00;
 UBRR0H=((baudselect(baud))>>8)&0x00ff;
 UBRR0L=(baudselect(baud))&0x00ff;
 UCSR0B=(1<<RXCIE1)|(1<<RXEN1)|(1<<TXEN1);
 UCSR0C=(3<<UCSZ10);
 SEI(); //re-enable interrupts
 ReceLen=0;
 OldReceLen=0;
 ReceLenCount=0;
} 
//===========================================================================

#pragma interrupt_handler uart0_rx_isr:19
void uart0_rx_isr(void) //uart has received a character in UDR
{
	uchar i;
//	char input = UDR1;
	char input = UDR0;
    if(input =='\b')
    {   
        if(cmd_index == 0) return;
		else
		{
		 USART_Transmit_byte(input);
		 Transmit_arraydata(" ");
		 cmd_index--;
		 CmdBuffer[cmd_index]='\0';
		 USART_Transmit_byte(input);	
		 return;
		}
     }
	
	else if((input == '\r')||(input == '\n')||(input == '\r\n'))	  //开始处理命令(input == "\r\n")需要再改善
	 {
	    USART_Transmit_byte(0x0d);   //  change 2013-11-03
	    USART_Transmit_byte(0x0a);
	  	receiveover=1;
		return;
	 }
	 
	 USART_Transmit_byte(input);	 
	 CmdBuffer[cmd_index++]=input;
	 
     if(cmd_index > 60)
	 {
		receiveover = 1;
	 }	
	
}

/***send 1byte**********/

void USART_Transmit_byte(uchar bytedata)
{
 while(!(UCSR0A&(1<<UDRE1)));
 UDR0=bytedata;
}
/*
void USART_Transmit_byte(uchar bytedata)
{
 while(!(UCSR1A&(1<<UDRE1)));
 UDR1=bytedata;
}
*/
/***send array_byte data**********/

void Transmit_arraydata(uchar *point)
{
	uchar *p=point;
	while(*p!='\0')
	{
	USART_Transmit_byte(*p);
	p++;
	}
}
void Transmit_arraydataConst(const uchar *point)
{
	const uchar *p=point;
	while(*p!='\0')
	{
	USART_Transmit_byte(*p);
	p++;
	}
}
//==========================public help=========================================

const uchar	Help1[]={"Fixture Information\r\n"};
const uchar	Help2[]={"Help\r\n"};
const uchar	Help3[]={"Reset Fixture\r\n"};
const uchar Help4[]= {"Code VER.\r\n"};
const uchar Help5[]= {"Open North Magnetic Field\r\n"};
const uchar Help6[]= {"Close North Magnetic Field\r\n"};
const uchar Help7[]= {"Open South Magnetic Field\r\n"};
const uchar Help8[]= {"Close South Magnetic Field\r\n"};
const uchar Help9[]= {"Magnetic Field Up\r\n"};
const uchar Help10[]= {"Magnetic Field Down\r\n"};
const uchar Help11[]= {"Set Coil Current XXXX.XX\r\n"};
const uchar Help12[]= {"Set Current Offset XXX.XX\r\n"};
const uchar Help13[]= {"Set Fixture Ratio XXXX.XX\r\n"};
const uchar Help14[]= {"Calculate Fixture Ratio\r\n"};
const uchar Help15[]= {"Read Data\r\n"};
const uchar Help16[]= {"Read Coil Current\r\n"};
const uchar Help17[]= {"Read\r\n"};
const uchar Help18[]= {"Sample 1\r\n"};
const uchar Help19[]= {"Sample xxxxx (500-20000)\r\n"};
const uchar Help20[]= {"Sample magnetic\r\n"};
const uchar Help21[]= {"Read Compass data\r\n"};

/*
//====================fixture name and ver  hlep================================
const uchar qt[]=    {"Fixture Name:  P2 QT2 test station\r\n"};
const uchar ver[]= {"Vendor:  intelligent\r\n"};
const uchar fw[]= {"FW Version:  V1.0"};
*/
//====================fixture_id help===========================================
const uchar qt_id[]= {"Fixture ID:  P2 QT2 test station\r\n"};
const uchar Fixture_ID[]= {"Fixture ID:"};
const uchar Fixture_Name[]= {"Fixture Name:QT2\r\n"};
const uchar Shenzhen[]= {"Shenzhen Intelligent\r\n"};
const uchar FW_Version[]= {"FW Version: V1.00\r\n"};
const uchar Code[]= {"Code VER.=1.00\r\n"};
const uchar Alter_Date[]= {"Alter Date: 2015/6/11\r\n"};


//======================QT=========================================================

void ProgHelp(void)
{
		Transmit_arraydataConst(Help1);
		Transmit_arraydataConst(Help2);
		Transmit_arraydataConst(Help3);
		Transmit_arraydataConst(Help4);
		Transmit_arraydataConst(Help5);
		Transmit_arraydataConst(Help6);
		Transmit_arraydataConst(Help7);
		Transmit_arraydataConst(Help8);
		Transmit_arraydataConst(Help9);
		Transmit_arraydataConst(Help10);
		Transmit_arraydataConst(Help11);
		Transmit_arraydataConst(Help12);
		Transmit_arraydataConst(Help13);
		Transmit_arraydataConst(Help14);
		Transmit_arraydataConst(Help15);
		Transmit_arraydataConst(Help16);
		Transmit_arraydataConst(Help17);
		Transmit_arraydataConst(Help18);
		Transmit_arraydataConst(Help19);
		Transmit_arraydataConst(Help20);
		Transmit_arraydataConst(Help21);
}

//==============================================================================

void ProgFixture_ID(void)
{
		//Transmit_arraydataConst(Fixture_ID);
		Transmit_arraydataConst(Fixture_Name);
		Transmit_arraydataConst(Shenzhen);
		Transmit_arraydataConst(FW_Version);
		Transmit_arraydataConst(Code);
		Transmit_arraydataConst(Alter_Date);
	//	Transmit_arraydataConst(qt_id);
	//	Transmit_arraydataConst(qt_id);
}



//======================74HC595=================================================
/*
void HC595_write(uchar data)
{
   uchar i=0,j;
	SCK_595_L;
   RCK_595_L;
   for(i=0;i<8;i++)
   {
      if(0x80&data)
	  DATA_595_H;
	  else 
	  DATA_595_L;
	  SCK_595_H;
	  delayms(5);
	  SCK_595_L;
	  data<<=1;
   }
   RCK_595_H;  //产生一个上升沿的输出脉冲
   delayms(2);
}
*/
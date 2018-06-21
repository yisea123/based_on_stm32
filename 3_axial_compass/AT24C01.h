/************************************************
文件：AT24C01.h
用途：AT24C01头文件
************************************************/
#ifndef __AT24C01_H__
#define __AT24C01_H__

//------在此设定芯片型号------
#define e2prom 512		// <---在此设定芯片型号, 1代表24C01; 16代表24C16; 512代表24C512

#if e2prom==1
	#define PAGE_SIZE 8
	#define SIZE 0x007f
#elif e2prom==2
	#define PAGE_SIZE 8
	#define SIZE 0x00ff
#elif e2prom==4
	#define PAGE_SIZE 16
	#define SIZE 0x01ff
#elif e2prom==8
	#define PAGE_SIZE 16
	#define SIZE 0x03ff
#elif e2prom==16
	#define PAGE_SIZE 16
	#define SIZE 0x07ff
#elif e2prom==32
	#define PAGE_SIZE 32
	#define SIZE 0x0fff
#elif e2prom==64
	#define PAGE_SIZE 32
	#define SIZE 0x1fff
#elif e2prom==128
	#define PAGE_SIZE 64
	#define SIZE 0x3fff
#elif e2prom==256
	#define PAGE_SIZE 64
	#define SIZE 0x7fff
#elif e2prom==512
	#define PAGE_SIZE 128
	#define SIZE 0xffff
#endif

#define SLAW 0x18

//--------在此设定芯片地址-------
#define W_ADD_COM 0xa0	//写字节命令及器件地址(根据地址实际情况改变), 1010 A2 A1 A0 0
#define R_ADD_COM 0xa1	//读命令字节及器件地址(根据地址实际情况改变), 1010 A2 A1 A0 1

#define ERR_SLAW	1	//写字节命令及器件地址错, 在此也就是读写器件错!!

extern unsigned char * wt24c_fc(unsigned char *p, unsigned int ad, unsigned char n);
extern void wt24c(unsigned char *p_rsc, unsigned int ad_dst, unsigned int num);
extern void rd24c(unsigned char *p_dst, unsigned int ad_rsc, unsigned int num);

#endif
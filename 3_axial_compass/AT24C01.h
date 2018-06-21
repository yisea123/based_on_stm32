/************************************************
�ļ���AT24C01.h
��;��AT24C01ͷ�ļ�
************************************************/
#ifndef __AT24C01_H__
#define __AT24C01_H__

//------�ڴ��趨оƬ�ͺ�------
#define e2prom 512		// <---�ڴ��趨оƬ�ͺ�, 1����24C01; 16����24C16; 512����24C512

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

//--------�ڴ��趨оƬ��ַ-------
#define W_ADD_COM 0xa0	//д�ֽ����������ַ(���ݵ�ַʵ������ı�), 1010 A2 A1 A0 0
#define R_ADD_COM 0xa1	//�������ֽڼ�������ַ(���ݵ�ַʵ������ı�), 1010 A2 A1 A0 1

#define ERR_SLAW	1	//д�ֽ����������ַ��, �ڴ�Ҳ���Ƕ�д������!!

extern unsigned char * wt24c_fc(unsigned char *p, unsigned int ad, unsigned char n);
extern void wt24c(unsigned char *p_rsc, unsigned int ad_dst, unsigned int num);
extern void rd24c(unsigned char *p_dst, unsigned int ad_rsc, unsigned int num);

#endif
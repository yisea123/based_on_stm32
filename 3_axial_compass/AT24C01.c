/************************************************
文件：AT24C01.c
用途：AT24C01操作函数
************************************************/
#include "main.h"
unsigned char syserr;

unsigned char * wt24c_fc(unsigned char *p, unsigned int ad, unsigned char n)
{
	unsigned char t=0;

	#if e2prom<32
	t=ad>>8;
	t<<=1;
	#endif
	
	TWI_Start();					
		
	if(TWI_Write(W_ADD_COM+t)==SLAW)
	{	
		#if e2prom>16
		TWI_Write(ad>>8);			
		#endif
		TWI_Write(ad);				
			
		for(;n>0;n--)			
		{
		    TWI_Write(*p);
			p++;
		}
	}
	else syserr=ERR_SLAW;		
	
	TWI_Stop();
    delayms(6);						
	
	return(p);
}

void wt24c(unsigned char *p_rsc, unsigned int ad_dst, unsigned int num)
{
    unsigned int n;
//	printf("addr is%d\r\n",ad_dst);
    n=ad_dst/PAGE_SIZE;		
	if(n) n=(unsigned long)PAGE_SIZE*(n+1)-ad_dst;
	else n=PAGE_SIZE-ad_dst;
	
    if(n>=num)		
    {
	 	wt24c_fc(p_rsc, ad_dst, num);
    	if(syserr!=0) return;
    }
    else			
	    {
	    p_rsc=wt24c_fc(p_rsc, ad_dst, n);
    	if(syserr!=0) return;
		
		num-=n;     
        ad_dst+=n;	

       
        while(num>=PAGE_SIZE)	
        {
		 	p_rsc=wt24c_fc(p_rsc, ad_dst, PAGE_SIZE);
        	if(syserr!=0) return;
        	
            num-=PAGE_SIZE;		
        	ad_dst+=PAGE_SIZE;	
		}
        
		if(num)			
			wt24c_fc(p_rsc, ad_dst, num);
    }
}
//=============================================================
void rd24c(unsigned char *p_dst, unsigned int ad_rsc, unsigned int num)
{
    unsigned char t=0;
//	printf("addr is%d\r\n",ad_rsc);

	#if e2prom<32
	t=ad_rsc>>8;
	t<<=1;
	#endif
	
	TWI_Start();					
		
	if(TWI_Write(W_ADD_COM+t)==SLAW)
	{	
		#if e2prom>16
		TWI_Write(ad_rsc>>8);	
		#endif
		TWI_Write(ad_rsc);			
				
		TWI_Start();				
		TWI_Write(R_ADD_COM+t);		
				
		for(;num>1;num--)
		{
		    *p_dst=i2c_read(1);		
			p_dst++;
		}
		*p_dst=i2c_read(0);		
			p_dst++;
	}
	else syserr=ERR_SLAW;		
	//delayms(6);	
	TWI_Stop();	
}
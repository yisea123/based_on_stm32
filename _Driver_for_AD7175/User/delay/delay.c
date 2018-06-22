#include "delay.h"
						   
void delay_us(u32 nus)
{		
	 uint8_t countertimer = 20;
    for(; nus > 0; nus--)
    {
        do
        {
            countertimer--;
        }while(countertimer == 0);
    }
    
}

void delay_ms(u16 nms)
{	 		  	  
    for(; nms > 0; nms--)
    {
        delay_us(1000);
    }
}
			 




































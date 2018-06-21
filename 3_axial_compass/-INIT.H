//ICC-AVR application builder : 
// Target : M64
// Crystal: 7.3728M


#ifndef __INT_H__
#define __INT_H__

    
//=============================================================================
#define   Set_South_Static	 		  	 	setbit(PORTB,PB4)  
#define   Clr_South_Static	 		   		clrbit(PORTB,PB4) 
#define   Set_Nouth_Static	 		  	 	setbit(PORTB,PB5)  
#define   Clr_Nouth_Static	 		   		clrbit(PORTB,PB5) 

#define   UPKey 		    testbit(PING,1)  
#define   DownKey 		    testbit(PING,0)
#define   NorthKey		    testbit(PINC,PC0)
#define   SouthKey 		    testbit(PINC,PC1)

#define   CS2_H 		    setbit(PORTA,PA0) 
#define   CS2_L 		    clrbit(PORTA,PA0) 
#define   CS1_H 		    setbit(PORTA,PA1) 
#define   CS1_L 		    clrbit(PORTA,PA1)  
#define   SCL_H 		    setbit(PORTA,PA3) 
#define   SCL_L 		    clrbit(PORTA,PA3)  
#define   SDA_H 		    setbit(PORTA,PA2) 
#define   SDA_L 		    clrbit(PORTA,PA2)  

#define   DIN2_H   setbit(PORTF,PF2);
#define   DIN2_L   clrbit(PORTF,PF2);
#define   SCLK2_H  setbit(PORTF,PF3);
#define   SCLK2_L  clrbit(PORTF,PF3);
#define   SYNC2_H  setbit(PORTF,PF4);
#define   SYNC2_L  clrbit(PORTF,PF4);

#define   SetLED_Braod			setbit(PORTD,PD6)
#define   ClrLED_Braod 			clrbit(PORTD,PD6)

#define   INT 		    	testbit(PING,2)

void delayus(uint us);
extern void delayus(uint us);

#endif
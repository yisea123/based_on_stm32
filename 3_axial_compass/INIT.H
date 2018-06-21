//ICC-AVR application builder : 
// Target : M64
// Crystal: 7.3728M

#ifndef __INT_H__
#define __INT_H__



#define   Set_North_Magnetic 		    setbit(PORTB,PB6)  
#define   Clr_North_Magnetic 		    clrbit(PORTB,PB6)  //open
#define   Set_South_Magnetic 		    setbit(PORTB,PB7)  //switch
#define   Clr_South_Magnetic 		    clrbit(PORTB,PB7)  

#define   Set_ADG1404_A0 		    setbit(PORTG,PG3)  
#define   Clr_ADG1404_A0 		    clrbit(PORTG,PG3)
#define   Set_ADG1404_A1 		    setbit(PORTF,PF7)  
#define   Clr_ADG1404_A1 		    clrbit(PORTF,PF7)

#define		Set_HightV1      (Clr_ADG1404_A1,Clr_ADG1404_A0)
#define 	Set_HightV2      (Clr_ADG1404_A1,Set_ADG1404_A0)
#define 	Set_HightV3      (Set_ADG1404_A1,Clr_ADG1404_A0)
#define 	Set_HightV4      (Set_ADG1404_A1,Set_ADG1404_A0)

#define   UPKey 		    testbit(PING,1)  
#define   DownKey 		    testbit(PING,0)
#define   NorthKey		    testbit(PINC,PC0)
#define   SouthKey 		    testbit(PINC,PC1)

#define   DIN2_H   			setbit(PORTF,PF2);
#define   DIN2_L   			clrbit(PORTF,PF2);
#define   SCLK2_H  			setbit(PORTF,PF3);
#define   SCLK2_L  			clrbit(PORTF,PF3);
#define   SYNC2_H  			setbit(PORTF,PF4);
#define   SYNC2_L  			clrbit(PORTF,PF4);

#define   SetLED_R			setbit(PORTB,PB5)
#define   ClrLED_R 			clrbit(PORTB,PB5)
#define   SetLED_G			setbit(PORTB,PB4)
#define   ClrLED_G 			clrbit(PORTB,PB4)

#define   SetLED_Braod		setbit(PORTD,PD6)
#define   ClrLED_Braod 		clrbit(PORTD,PD6)

#define   INT 		    	testbit(PING,2)



void delayus(uint us);
extern void delayus(uint us);

#endif
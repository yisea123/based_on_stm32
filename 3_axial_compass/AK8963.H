#ifndef _AK8963_H_
#define _AK8963_H_


#define AK8963_add		0b00011000		


extern long X_Dat,Y_Dat,Z_Dat;			
extern long Ex_X_Dat,Ex_Y_Dat,Ex_Z_Dat;
extern long South_X,South_Y,South_Z;
extern long North_X,North_Y,North_Z;


void AK8963_init(void);
void AK8963_Wirte(uchar addr, uchar dat);
uchar AK8963_Read(uchar addr);
uint AK8963_Data_Processing(void);
uint AK8963_Smoothing(uchar time,uint ms);




#endif
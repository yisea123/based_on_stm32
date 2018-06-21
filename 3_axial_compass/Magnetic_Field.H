#ifndef _Magnetic_Field_H_
#define _Magnetic_Field_H_




extern uchar Flg_Updata;

extern uint magnetic_ratio;
extern uint magnetic_offset;
extern uchar Step_Key_Up;
extern uchar Step_Key_Down;
extern uchar FastTime_Key;
extern uchar Flg_CloseMagnetic;
extern long XSData,YSData,ZSData;
//extern ulong AD_sample[3];
//extern long NMS_sample[3];
extern uint AD_sample_X,AD_sample_Y,AD_sample_Z;
extern long NMS_sample_X,NMS_sample_Y,NMS_sample_Z;

extern void Magnetic_Field(uchar Sw,uchar Io);
extern void Cotrol_Coil_Current(float Current,uchar a);
extern void Key_Control_Up(void);
extern void Key_Control_Down(void);

extern float Read_Current(void);

extern long Read_NMS(uchar Switch_NS);
extern void Calculate_Sample_magnetic(void);



#endif
NOTE: AVR Code addresses are word addresses
All other addresses (including ones in FLASH) are byte addresses

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                        func_lit   008C   0038 =     56. bytes (rel,con,rom)

       Addr  Global Symbol
      -----  --------------------------------
       008C  __func_lit_start
       00C4  __func_lit_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                             lit   00C4   068A =   1674. bytes (rel,con,rom)

       Addr  Global Symbol
      -----  --------------------------------
       00C4  __lit_start
       00E4  _Fixture_Information
       00F8  _Help
       00FD  _Reset_Fixture
       010B  _Code_VER
       0115  _Open_North_Magnetic_Field
       012F  _Close_North_Magnetic_Field
       014A  _Open_South_Magnetic_Field
       0164  _Close_South_Magnetic_Field
       017F  _Magnetic_Field_Up
       0191  _Magnetic_Field_Down
       01A5  _Set_Coil_Current
       01B6  _Set_Current_Offset
       01C9  _Set_Fixture_Ratio
       01DB  _Calculate_Fixture_Ratio
       01F3  _Read_Data
       01FD  _Read_Coil_Current
       020F  _Read
       0214  _Sample_1
       021D  _Sample
       0224  _Sample_magnetic
       0234  _Read_Compass_data
       0246  _Set_A
       024C  _Set_B
       0252  _Set_DA
       0259  _Set_Info
       0262  _Set_Date
       026B  _Set_SN
       0272  _up
       0275  _down
       027A  _Test1
       0280  _Test2
       02A7  _Help1
       02BD  _Help2
       02C4  _Help3
       02D4  _Help4
       02E0  _Help5
       02FC  _Help6
       0319  _Help7
       0335  _Help8
       0352  _Help9
       0366  _Help10
       037C  _Help11
       0397  _Help12
       03B3  _Help13
       03CF  _Help14
       03E9  _Help15
       03F5  _Help16
       0409  _Help17
       0410  _Help18
       041B  _Help19
       0436  _Help20
       0448  _Help21
       045C  _qt_id
       047F  _Fixture_ID
       048B  _Fixture_Name
       049E  _Shenzhen
       04B5  _FW_Version
       04C9  _Code
       04DA  _Alter_Date
       055C  __ctype
       074E  __lit_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                           idata   074E   0521 =   1313. bytes (rel,con,rom)

       Addr  Global Symbol
      -----  --------------------------------
       074E  __idata_start
       0C6F  __idata_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                            text   0C70   6C0C =  27660. bytes (rel,con,rom)

       Addr  Global Symbol
      -----  --------------------------------
       0638  __start
       0638  __text_start
       065A  _exit
       065B  _ADC_init
       0669  _adc_converter
       0691  _adc_converter_multiple
       0740  _delayms
       0753  _delayus
       0764  _port_init
       0789  _init_devices
       07B2  _EEPROM_write
       07BB  _EEPROM_read
       07C2  _process
       07E4  _Write_AD5660
       086D  _ProgResponseEnd
       0877  _StrnCompare
       08CD  _DataCMP
       09FF  _ProgCommGo
       1045  _ProgComm
       1075  _getNunb
       10CD  _LED_Flash
       10F3  _Key_Pro
       1149  _main
       115D  _AK8963_init
       1239  _AK8963_Wirte
       1251  _AK8963_Read
       1269  _AK8963_Read_Data
       1290  _AK8963_Data_Processing
       137C  _AK8963_Smoothing
       14D4  _putchar$device_specific$
       14E9  _baudselect
       1532  _uart1_init
       155A  _uart0_rx_isr
       15CD  _USART_Transmit_byte
       15D1  _Transmit_arraydata
       15E1  _Transmit_arraydataConst
       15F1  _ProgHelp
       1630  _ProgFixture_ID
       163F  _delay
       1648  _AD5660Init
       1658  _Wr_AD5660
       16AB  _WaitACK
       16C1  _TWI_init
       16CF  _TWI_Start
       16D8  _TWI_Write
       16E7  _TWI_Read
       16EF  _i2c_read
       1708  _TWI_Stop
       170C  _Magnetic_Field
       1754  _Cotrol_Coil_Current
       17F2  _Key_Control_Up
       18D4  _Key_Control_Down
       19B9  _Read_Current
       1A19  _Read_NMS
       1B0C  _Calculate_Sample_magnetic
       1C25  _ADS8320_init
       1C2D  _Read_ADS8320
       1C72  _ADS8320_Smoothing
       1CCC  _TWI_Delay
       1CF7  _TWI_SDA_STATE
       1D01  _TWI_START
       1D0B  _TWI_STOP
       1D13  _TWI_SendACK
       1D1B  _TWI_SendNACK
       1D23  _TWI_WaitAck
       1D32  _TWI_SendByte
       1D50  _TWI_ReceiveByte
       1D67  _ads1110Config
       1D71  _RD_ADS
       1D8F  _get_ad_result
       1DEC  _EEPROM_Init
       1E46  _FtoC
       1E62  _CtoF
       1E80  _EEPROM_Write_Uint
       1E94  _EEPROM_Read_Uint
       1EA9  _Float_EEPROM_write
       1ED7  _Float_EEPROM_Read
       1F07  _E24C02_Write_Uint
       1F1F  _E24C02_Read_Uint
       1F3B  _Float_24C02_write
       1F54  _Float_24C02_Read
       1F6E  _bsp_InitAK8963
       1F7E  _GetAK8963Mag
       1F89  _Self_test
       1FD7  _AK8963_WriteByte
       1FEB  _AK8963_ReadByte
       2003  _AK8963_ReadData
       203B  _wt24c_fc
       2069  _wt24c
       20E1  _rd24c
       211D  __print
       25EB  _strlen
       25F7  push_arg4
       25F9  push_arg2
       25FC  asr32
       2608  mod16u
       260A  div16u
       260B  xdiv16u
       2624  div32u
       2626  mod32u
       262A  div32s
       262C  mod32s
       2658  long_div_prolog
       266B  long_div_epilog
       267B  tst_R16_R19
       2680  tst_R24_R27
       2685  neg_R24_R27
       268E  elpm32
       26A2  empy16s
       26B2  empy32u
       26B2  empy32s
       26E6  pop_xgset003C
       26EB  pop_xgset00FC
       26F2  pop_xgsetF0FC
       26FD  push_xgsetF0FC
       26FF  push_xgset30FC
       2701  push_xgset00FC
       2703  push_xgset003C
       2708  push_xgset300C
       270D  pop_xgset300C
       2712  push_xgsetF000
       2717  pop_xgsetF000
       271C  push_xgsetF00C
       2723  pop_xgsetF00C
       272A  push_xgset303C
       2731  pop_xgset303C
       2738  push_xgsetF03C
       2741  pop_xgsetF03C
       274A  xicall
       2753  neg32
       275C  long_prolog
       276A  long_epilog
       2775  tstzero1
       277B  tstzero2
       2781  lpm32
       2795  lsl32
       27A1  lsr16
       27A8  mpy16s
       27BC  asgncblkx
       27D5  fpsub
       27D7  fpadd
       27D8  fpadd_alt
       283F  fpadd1
       2849  fpadd1x
       2856  fpadd2
       2861  fpadd2x
       286F  fpsub1
       2879  fpsub2
       2884  fpsub1x
       2891  fpsub2x
       289F  fpint
       28D1  uint2fp
       28D3  int2fp
       28DC  ulong2fp
       28DE  long2fp
       28F5  fpdiv1
       28FE  fpdiv2
       2908  fpdiv1x
       2914  fpdiv2x
       298F  fpmul
       29D9  fpmul1
       29E3  fpmul2
       29EE  fpmul1x
       29FB  fpmul2x
       2A09  unpacks
       2A11  tstR16_R19
       2A19  unpacku
       2A32  normalize_and_pack
       2A67  pack
       2A77  saveFPRegs
       2A86  restoreFPRegs
       2A95  fpmule
       2AE8  fpmule1
       2AF2  fpmule2
       2AFD  fpmule2x
       2B0B  fpcmp
       2B18  fpcmp1
       2B23  fpcmp2
       2B2F  fpneg
       2B37  _cstrcpy
       2B41  _cstrlen
       2B4E  _atoi
       2BA5  _itoa
       2C2C  _ltoa
       2CFA  _ultoa
       2D76  _utoa
       2DCF  _atof
       2F22  _powi
       2F56  _ftoa
       310F  _isdigit
       311D  _islower
       312B  _isspace
       3139  _toupper
       3150  _expf
       3262  _floorf
       32C5  _frexpf
       3337  _ldexpf
       33AC  _logf
       34A2  _powf
       3567  _sqrtf
       36BF  _printf
       36F8  __FormatFP_1
       3C3E  __text_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                            data   0100   0521 =   1313. bytes (rel,con,ram)

       Addr  Global Symbol
      -----  --------------------------------
       0100  __data_start
       0100  _SY
       0104  _error
       0132  _step
       04E1  _CmdBuffer
       0521  _CmdBuffer2
       0561  _CmdBuffer1
       05A1  _CmdBuffer0
       05E3  _Flg_Updata
       0621  __data_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                             bss   0621   0137 =    311. bytes (rel,con,ram)

       Addr  Global Symbol
      -----  --------------------------------
       0621  __bss_start
       0621  _North_Float
       0625  _South_Float
       0629  _TableTest
       062D  _Tem3_float
       0631  _Tem2_float
       0635  _Tem1_float
       0639  _Cont
       063D  _FID_Buffer
       0660  _LED_Time
       0662  _Tem_Valu
       0666  _LED_Time_Flg
       0667  _SampleSave
       0668  _Tem2_long
       066C  _Tem1_long
       0670  _read_buff
       0674  _write_buff
       0678  _a
       0684  _Z_Calibrate
       0688  _Y_Calibrate
       068C  _X_Calibrate
       0690  _Z_Data
       0692  _Y_Data
       0694  _X_Data
       0696  _SeptSouthNorth
       0697  _North_Z
       069B  _North_Y
       069F  _North_X
       06A3  _South_Z
       06A7  _South_Y
       06AB  _South_X
       06AF  _Ex_Z_Dat
       06B3  _Ex_Y_Dat
       06B7  _Ex_X_Dat
       06BB  _Flg_Uart
       06BC  _Freq
       06BE  _Module
       06BF  _ReceLen
       06C0  _ReceLenCount
       06C1  _OldReceLen
       06C2  _cmd_index0
       06C3  _cmd_index
       06C4  _receiveover0
       06C5  _receiveover
       06C6  _PointReceLen
       06C8  _ad5660_wr_buf
       06CB  _Count
       06CC  _Key_Tem
       06D0  _NMS_sample_Z
       06D4  _NMS_sample_Y
       06D8  _NMS_sample_X
       06DC  _AD_sample_Z
       06DE  _AD_sample_Y
       06E0  _AD_sample_X
       06E2  _ZSData
       06E6  _YSData
       06EA  _XSData
       06EE  _Flg_CloseMagnetic
       06EF  _FastTime_Key
       06F0  _Step_Key_Down
       06F1  _Step_Key_Up
       06F2  _AD_Result
       070C  _x
       0710  _g_tMag
       072B  _syserr
       0754  __textmode
       0756  _errno
       0758  __bss_end

Area                               Start  End    Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                          vector   0000   004C =     76. bytes (abs,ovr,rom)

Files Linked      [ module(s) ]

C:\iccv7avr\lib\crtatmega.o	[ crtatmega.s ]
adc.o	[ adc.c ]
main.o	[ main.c ]
ak8963.o	[ ak8963.c ]
uart.o	[ uart.c ]
AD5660.o	[ AD5660.c ]
twi.o	[ twi.c ]
Magnetic_Field.o	[ Magnetic_Field.c ]
ADS8320.o	[ ADS8320.c ]
ADS1110.o	[ ADS1110.c ]
Eeprom.o	[ Eeprom.c ]
uAK8963.o	[ uAK8963.c ]
AT24C01.o	[ AT24C01.c ]
<library>	[ _printf.s, strlen.s, asave.s, asr32.s, div16u.s, div32.s, elpm32.s, emul16s.s, emul32.s, gpop2.s, gpop3.s ]
[ gpop5.s, gpush.s, gpushx.s, gpushy.s, gpushz.s, gpusha.s, gpushb.s, icall.s, lneg.s, longutil.s, lpm32.s ]
[ lsl32.s, lsr16.s, mpy16s.s, pmstructx.s, fpadd.s, fpadd1.s, fpadd1x.s, fpadd2.s, fpadd2x.s, fpsub1.s ]
[ fpsub2.s, fpsub1x.s, fpsub2x.s, fpint.s, int2fp.s, fpdiv_new.s, fpmulnew.s, fpmul1.s, fpmul2.s, fpmul1x.s ]
[ fpmul2x.s, fputil.s, fpmule.s, fpmule1.s, fpmule2.s, fpmule2x.s, fpcmp.s, fpcmp1.s, fpcmp2.s, fpneg.s ]
[ cstrcpy.s, cstrlen.s, atoi.s, itoa.s, ltoa.s, ultoa.s, utoa.s, atof.s, ftoa.s, isdigit.s, islower.s, isspace.s ]
[ ctype.s, toupper.s, expf.s, floorf.s, frexpf.s, ldexpf.s, logf.s, powf.s, sqrtf.s, printf.s, textmode.s ]
[ formatfp1.s, errno.s ]

User Global Definitions

ram_end = 0x10ff
hwstk_size = 0x1e

User Base Address Definitions

func_lit = 0x8c
data = 0x100
eeprom:0.2048


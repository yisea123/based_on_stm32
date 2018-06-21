# Driver for DDS Chip AD9834

#### Hardware Description###
- control MCU: STM32F103ZET6, Footprint: LQFP:144Pin
- Development IDE: MDK V5.12
- simulation tools: J-link V9

### Project Description
- Driver for DDS chip AD9834
- contian function:


### User manual
- contian AD9834 and delay
- output only need call "AD9834_OutputSet(FreqRegisterNumber, OutputFrequency, PhaseDegree);".
- after call sine wave will show out few millisecond.
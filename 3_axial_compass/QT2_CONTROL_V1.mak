CC = iccavr
LIB = ilibw
CFLAGS =  -e -D__ICC_VERSION=722 -DATMega64  -l -g -MLongJump -MHasMul -MEnhanced -Wf-use_elpm -Wf-const_is_flash -DCONST="" 
ASFLAGS = $(CFLAGS) 
LFLAGS =  -g -e:0x10000 -ucrtatmega.o -bfunc_lit:0x8c.0x10000 -dram_end:0x10ff -bdata:0x100.0x10ff -dhwstk_size:30 -beeprom:0.2048 -fihx_coff -S2
FILES = adc.o main.o ak8963.o uart.o AD5660.o twi.o Magnetic_Field.o ADS8320.o ADS1110.o Eeprom.o uAK8963.o AT24C01.o 

QT2_CONTROL_V1:	$(FILES)
	$(CC) -o QT2_CONTROL_V1 $(LFLAGS) @QT2_CONTROL_V1.lk   -lfpatmega -lcatmega
adc.o: .\main.h C:\iccv7avr\include\stdlib.h C:\iccv7avr\include\_const.h C:\iccv7avr\include\limits.h C:\iccv7avr\include\iom64v.h C:\iccv7avr\include\macros.h C:\iccv7avr\include\AVRdef.h C:\iccv7avr\include\string.h C:\iccv7avr\include\stdio.h C:\iccv7avr\include\stdarg.h .\INIT.H .\UART.H .\adc.h .\AD5660.h .\ak8963.h C:\iccv7avr\include\math.h .\twi.h .\Magnetic_Field.H .\ADS8320.h .\ADS1110.h .\Eeprom.h .\AT24C01.h .\uAk8963.h
adc.o:	adc.c
	$(CC) -c $(CFLAGS) adc.c
main.o: .\main.h C:\iccv7avr\include\stdlib.h C:\iccv7avr\include\_const.h C:\iccv7avr\include\limits.h C:\iccv7avr\include\iom64v.h C:\iccv7avr\include\macros.h C:\iccv7avr\include\AVRdef.h C:\iccv7avr\include\string.h C:\iccv7avr\include\stdio.h C:\iccv7avr\include\stdarg.h .\INIT.H .\UART.H .\adc.h .\AD5660.h .\ak8963.h C:\iccv7avr\include\math.h .\twi.h .\Magnetic_Field.H .\ADS8320.h .\ADS1110.h .\Eeprom.h .\AT24C01.h .\uAk8963.h .\Commands.H
main.o:	main.c
	$(CC) -c $(CFLAGS) main.c
ak8963.o: .\main.h C:\iccv7avr\include\stdlib.h C:\iccv7avr\include\_const.h C:\iccv7avr\include\limits.h C:\iccv7avr\include\iom64v.h C:\iccv7avr\include\macros.h C:\iccv7avr\include\AVRdef.h C:\iccv7avr\include\string.h C:\iccv7avr\include\stdio.h C:\iccv7avr\include\stdarg.h .\INIT.H .\UART.H .\adc.h .\AD5660.h .\ak8963.h C:\iccv7avr\include\math.h .\twi.h .\Magnetic_Field.H .\ADS8320.h .\ADS1110.h .\Eeprom.h .\AT24C01.h .\uAk8963.h
ak8963.o:	ak8963.c
	$(CC) -c $(CFLAGS) ak8963.c
uart.o: .\main.h C:\iccv7avr\include\stdlib.h C:\iccv7avr\include\_const.h C:\iccv7avr\include\limits.h C:\iccv7avr\include\iom64v.h C:\iccv7avr\include\macros.h C:\iccv7avr\include\AVRdef.h C:\iccv7avr\include\string.h C:\iccv7avr\include\stdio.h C:\iccv7avr\include\stdarg.h .\INIT.H .\UART.H .\adc.h .\AD5660.h .\ak8963.h C:\iccv7avr\include\math.h .\twi.h .\Magnetic_Field.H .\ADS8320.h .\ADS1110.h .\Eeprom.h .\AT24C01.h .\uAk8963.h
uart.o:	uart.c
	$(CC) -c $(CFLAGS) uart.c
AD5660.o: .\main.h C:\iccv7avr\include\stdlib.h C:\iccv7avr\include\_const.h C:\iccv7avr\include\limits.h C:\iccv7avr\include\iom64v.h C:\iccv7avr\include\macros.h C:\iccv7avr\include\AVRdef.h C:\iccv7avr\include\string.h C:\iccv7avr\include\stdio.h C:\iccv7avr\include\stdarg.h .\INIT.H .\UART.H .\adc.h .\AD5660.h .\ak8963.h C:\iccv7avr\include\math.h .\twi.h .\Magnetic_Field.H .\ADS8320.h .\ADS1110.h .\Eeprom.h .\AT24C01.h .\uAk8963.h
AD5660.o:	AD5660.c
	$(CC) -c $(CFLAGS) AD5660.c
twi.o: .\main.h C:\iccv7avr\include\stdlib.h C:\iccv7avr\include\_const.h C:\iccv7avr\include\limits.h C:\iccv7avr\include\iom64v.h C:\iccv7avr\include\macros.h C:\iccv7avr\include\AVRdef.h C:\iccv7avr\include\string.h C:\iccv7avr\include\stdio.h C:\iccv7avr\include\stdarg.h .\INIT.H .\UART.H .\adc.h .\AD5660.h .\ak8963.h C:\iccv7avr\include\math.h .\twi.h .\Magnetic_Field.H .\ADS8320.h .\ADS1110.h .\Eeprom.h .\AT24C01.h .\uAk8963.h
twi.o:	twi.c
	$(CC) -c $(CFLAGS) twi.c
Magnetic_Field.o: .\main.h C:\iccv7avr\include\stdlib.h C:\iccv7avr\include\_const.h C:\iccv7avr\include\limits.h C:\iccv7avr\include\iom64v.h C:\iccv7avr\include\macros.h C:\iccv7avr\include\AVRdef.h C:\iccv7avr\include\string.h C:\iccv7avr\include\stdio.h C:\iccv7avr\include\stdarg.h .\INIT.H .\UART.H .\adc.h .\AD5660.h .\ak8963.h C:\iccv7avr\include\math.h .\twi.h .\Magnetic_Field.H .\ADS8320.h .\ADS1110.h .\Eeprom.h .\AT24C01.h .\uAk8963.h
Magnetic_Field.o:	Magnetic_Field.c
	$(CC) -c $(CFLAGS) Magnetic_Field.c
ADS8320.o: .\main.h C:\iccv7avr\include\stdlib.h C:\iccv7avr\include\_const.h C:\iccv7avr\include\limits.h C:\iccv7avr\include\iom64v.h C:\iccv7avr\include\macros.h C:\iccv7avr\include\AVRdef.h C:\iccv7avr\include\string.h C:\iccv7avr\include\stdio.h C:\iccv7avr\include\stdarg.h .\INIT.H .\UART.H .\adc.h .\AD5660.h .\ak8963.h C:\iccv7avr\include\math.h .\twi.h .\Magnetic_Field.H .\ADS8320.h .\ADS1110.h .\Eeprom.h .\AT24C01.h .\uAk8963.h
ADS8320.o:	ADS8320.c
	$(CC) -c $(CFLAGS) ADS8320.c
ADS1110.o: .\main.h C:\iccv7avr\include\stdlib.h C:\iccv7avr\include\_const.h C:\iccv7avr\include\limits.h C:\iccv7avr\include\iom64v.h C:\iccv7avr\include\macros.h C:\iccv7avr\include\AVRdef.h C:\iccv7avr\include\string.h C:\iccv7avr\include\stdio.h C:\iccv7avr\include\stdarg.h .\INIT.H .\UART.H .\adc.h .\AD5660.h .\ak8963.h C:\iccv7avr\include\math.h .\twi.h .\Magnetic_Field.H .\ADS8320.h .\ADS1110.h .\Eeprom.h .\AT24C01.h .\uAk8963.h
ADS1110.o:	ADS1110.c
	$(CC) -c $(CFLAGS) ADS1110.c
Eeprom.o: .\main.h C:\iccv7avr\include\stdlib.h C:\iccv7avr\include\_const.h C:\iccv7avr\include\limits.h C:\iccv7avr\include\iom64v.h C:\iccv7avr\include\macros.h C:\iccv7avr\include\AVRdef.h C:\iccv7avr\include\string.h C:\iccv7avr\include\stdio.h C:\iccv7avr\include\stdarg.h .\INIT.H .\UART.H .\adc.h .\AD5660.h .\ak8963.h C:\iccv7avr\include\math.h .\twi.h .\Magnetic_Field.H .\ADS8320.h .\ADS1110.h .\Eeprom.h .\AT24C01.h .\uAk8963.h
Eeprom.o:	Eeprom.c
	$(CC) -c $(CFLAGS) Eeprom.c
uAK8963.o: .\main.h C:\iccv7avr\include\stdlib.h C:\iccv7avr\include\_const.h C:\iccv7avr\include\limits.h C:\iccv7avr\include\iom64v.h C:\iccv7avr\include\macros.h C:\iccv7avr\include\AVRdef.h C:\iccv7avr\include\string.h C:\iccv7avr\include\stdio.h C:\iccv7avr\include\stdarg.h .\INIT.H .\UART.H .\adc.h .\AD5660.h .\ak8963.h C:\iccv7avr\include\math.h .\twi.h .\Magnetic_Field.H .\ADS8320.h .\ADS1110.h .\Eeprom.h .\AT24C01.h .\uAk8963.h
uAK8963.o:	uAK8963.c
	$(CC) -c $(CFLAGS) uAK8963.c
AT24C01.o: .\main.h C:\iccv7avr\include\stdlib.h C:\iccv7avr\include\_const.h C:\iccv7avr\include\limits.h C:\iccv7avr\include\iom64v.h C:\iccv7avr\include\macros.h C:\iccv7avr\include\AVRdef.h C:\iccv7avr\include\string.h C:\iccv7avr\include\stdio.h C:\iccv7avr\include\stdarg.h .\INIT.H .\UART.H .\adc.h .\AD5660.h .\ak8963.h C:\iccv7avr\include\math.h .\twi.h .\Magnetic_Field.H .\ADS8320.h .\ADS1110.h .\Eeprom.h .\AT24C01.h .\uAk8963.h
AT24C01.o:	AT24C01.c
	$(CC) -c $(CFLAGS) AT24C01.c

# Movement Controllor
@(movement)[controller, Markdown]

---
[TOC]

---
## Purpose:

* As a slave device can analysis master device protocols and do movements through CAN BUS port.

## Platform

- MCU: `STM32F105V8T6 `
- IDE: ` MDK 5.24 `



## Hardware Port List:

* CAN: 2 port.

* RS232: 1 port.

* RS485 / RS422: 1port can compatible  with RS485 and RS422 port.

* Digital Input Port: 6 input port compatible with NPN and PNP input, Voltage input range from 12 to 36V.

* Digital Output Port: 6 output port using PNP output mode.

* Encoder Port: 1 encoder port for 5V encoder device.

* H-Bridge port: 1 Brushed DC Motor port for 12 ~24V DC brushed motor. Voltage reference from AC200 power voltage, Current limited in 5A.

	               			        

## Test Loop:



    

## Change Log

- 2017-12-23: Test Version completed
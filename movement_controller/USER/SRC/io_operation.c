/**
 * IO port 
*/
#include "io_operation.h"
/**
 * @brief: IO_Port configurate.
 *          default status: output disable.
 * @param:
 * @return: 
 */
void _IO_PortInit(void)
{
    GPIO_InitTypeDef GPIO_InitStruction;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);
    
    /*-------------- Input Configuration ----------------*/
    GPIO_InitStruction.GPIO_Pin   = _IO_INPUT_PIN_1 | _IO_INPUT_PIN_2 | _IO_INPUT_PIN_3 |\
                                    _IO_INPUT_PIN_4 | _IO_INPUT_PIN_5;
    GPIO_InitStruction.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_InitStruction.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PORTC, &GPIO_InitStruction);

    GPIO_InitStruction.GPIO_Pin = _IO_INPUT_PIN_6;
    GPIO_Init(PORTD, &GPIO_InitStruction);

    /*-------------- Output Configuration ----------------*/
    GPIO_InitStruction.GPIO_Pin   = _IO_OUTPUT_PIN_1 | _IO_OUTPUT_PIN_2 | _IO_OUTPUT_PIN_3 |\
                                    _IO_OUTPUT_PIN_4 | _IO_OUTPUT_PIN_5 | _IO_OUTPUT_PIN_6;
    GPIO_InitStruction.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStruction.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PORTC, &GPIO_InitStruction);
}

/**
 * @brief: Read Input bit value.
 * @param: pin number need to be read.
 * @return: return the pin status(pin value).
 */
uint8_t ReadSinglePin(uint8_t pinNumber)
{
    uint8_t pinValueTemp = 0;
    switch (pinNumber)
    {
		default:
		{
			pinValueTemp = 0xFF;
			break;
		}
        case (1):
        {
            pinValueTemp = GPIO_ReadInputDataBit(PORTC, _IO_INPUT_PIN_1);
        }break;
        case (2):
        {
            pinValueTemp = GPIO_ReadInputDataBit(PORTC, _IO_INPUT_PIN_2);
        }break;
        case (3):
        {
            pinValueTemp = GPIO_ReadInputDataBit(PORTC, _IO_INPUT_PIN_3);
        }break;
        case (4):
        {
            pinValueTemp = GPIO_ReadInputDataBit(PORTC, _IO_INPUT_PIN_4);
        }break;
        case (5):
        {
            pinValueTemp = GPIO_ReadInputDataBit(PORTC, _IO_INPUT_PIN_5);
        }break;
        case (6):
        {
            pinValueTemp = GPIO_ReadInputDataBit(PORTD, _IO_INPUT_PIN_6);
        }break;
    }
    return pinValueTemp;
}

/**
 * @brief:  set single IO status.
 * @param:  write_pin is specifies the pin need to be written.
 * @param:  writeValue only can be set to 0 or 1, @refer BitAction define.
 * @return: return the write status, success or ERROR, @refer ErrorStatus define.
 */
ErrorStatus WriteSinglePin(uint8_t write_Pin, uint8_t writeValue)
{
    ErrorStatus errorCode;
    switch (write_Pin)
    {
        case (1):
        {
			if(writeValue)
				GPIO_SetBits(PORTC, _IO_OUTPUT_PIN_1);
			else
				GPIO_ResetBits(PORTC, _IO_OUTPUT_PIN_1);
            errorCode = SUCCESS;
        }break;
        case (2):
        {
			if(writeValue)
				GPIO_SetBits(PORTC, _IO_OUTPUT_PIN_2);
			else
				GPIO_ResetBits(PORTC, _IO_OUTPUT_PIN_2);
            errorCode = SUCCESS;
        }break;
        case (3):
        {
			if(writeValue)
				GPIO_SetBits(PORTC, _IO_OUTPUT_PIN_3);
			else
				GPIO_ResetBits(PORTC, _IO_OUTPUT_PIN_3);
            errorCode = SUCCESS;
        }break;
        case (4):
        {
			if(writeValue)
				GPIO_SetBits(PORTC, _IO_OUTPUT_PIN_4);
			else
				GPIO_ResetBits(PORTC, _IO_OUTPUT_PIN_4);
            errorCode = SUCCESS;
        }break;
        case (5):
        {
			if(writeValue)
				GPIO_SetBits(PORTC, _IO_OUTPUT_PIN_5);
			else
				GPIO_ResetBits(PORTC, _IO_OUTPUT_PIN_5);
            errorCode = SUCCESS;
        }break;
        case (6):
        {
			if(writeValue)
				GPIO_SetBits(PORTC, _IO_OUTPUT_PIN_6);
			else
				GPIO_ResetBits(PORTC, _IO_OUTPUT_PIN_6);
            errorCode = SUCCESS;
        }break;

        default:
            errorCode = ERROR;
            break;
    }
    return errorCode;
}
/**
 * @brief: read multi pin value, can be set start and end pin number. 
 *         only for input pins.
 * @param: startLabel, define read function from which pin to start to read.
 * @param: endLabel, define read function stop read place.
 *         attention: 1. startLabel and endLabel less than full input pins.
 *                    2. startLabel less or equal than endLabel.
 * @return: 1. return the read data, every bit mark a pin status, from 1 to 6.
 *          2. if return 0xff, means the read protocols was damaged. Try Again.
 */
uint8_t ReadMultiPin(uint8_t startLabel, uint8_t endLabel)
{
    uint8_t readValueTemp = 0;
    uint8_t functionTemp = 1;

    if ((startLabel <= endLabel) && (endLabel <= _IO_INPUT_NUMBER))
    {
        if(endLabel !=_IO_INPUT_NUMBER)
        {
            readValueTemp = GPIO_ReadInputData(PORTC);
            readValueTemp = readValueTemp >> (startLabel + 8);      // get data from pin9 to pin15
            for(; startLabel <= endLabel; startLabel++)             // let length bit become 1.
            {
                functionTemp = functionTemp << 1;
                functionTemp += 1;
            }
            readValueTemp = readValueTemp & functionTemp;
        }
        else
        {
            readValueTemp = GPIO_ReadInputData(PORTC);
            readValueTemp = readValueTemp >> (startLabel + 8); // get data from pin9 to pin15
            for (; startLabel <= endLabel; startLabel++)       // let length bit become 1.
            {
                functionTemp = functionTemp << 1;
                functionTemp += 1;
            }
            readValueTemp = readValueTemp & functionTemp;
            readValueTemp = readValueTemp + (uint8_t)(1 << (_IO_INPUT_NUMBER - 1));  // add _IO_INPUT_PIN_6
        }
        return readValueTemp;
    }
    else
        return 0xff;
}

/**
 * @brief: write multi pin function, like read multi pin function.
 *         AC200 configurated the output pin is using PC0 -> PC5 as Digital output pin DO_1 -> DO_5.
 * @param: startLabel and endLabel like read multi pin define.
 * @return: return written answer, error or success
 */
ErrorStatus WriteMultiPin(uint8_t startLabel, uint8_t endLabel, uint8_t _IO_Param)
{
    // uint8_t labelTemp = startLabel;

    // if(startLabel <= endLabel)
    // {
    //     for (; labelTemp <= endLabel; labelTemp)
    //         GPIO_SetBits(PORTC, )
    // }
    // else
    // {
        return ERROR;
    // }

}




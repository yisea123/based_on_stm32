#ifndef __adc_h__
#define __adc_h__

#define AMP 63.3
#define ADC_CLOSE() ADCSRA = 0

//void ADC_init(unsigned char ADC_Channel);
void ADC_init(void);

unsigned int adc_converter(void);
unsigned int adc_converter_multiple(void);

#endif
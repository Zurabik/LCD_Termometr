/*
 * adc_megaxx8.h
 *
 * Created: 01.08.2015 14:16:01
 *  Author: Vadim2
 */ 


#ifndef ADC_MEGAXX8_H_
#define ADC_MEGAXX8_H_

#define 	ADC_ADTS_REG   ADCSRB

enum  	adc_mux_setting {
	ADC_MUX_ADC0 = 0x0,
	ADC_MUX_ADC1 = 0x1,
	ADC_MUX_ADC2 = 0x2,
	ADC_MUX_ADC3 = 0x3,
	ADC_MUX_ADC4 = 0x4,
	ADC_MUX_ADC5 = 0x5,
	ADC_MUX_ADC6 = 0x6,
	ADC_MUX_ADC7 = 0x7,
	ADC_MUX_1V1 = 0x0E,
	ADC_MUX_GND = 0x0F,
};

enum  	adc_voltage_reference {
	ADC_VREF_AREF = (0x0 << REFS0),
	ADC_VREF_AVCC = (0x1 << REFS0),
	ADC_VREF_RESERVED = (0x2 << REFS0),
	ADC_VREF_1V1 = (0x3 << REFS0),
};
#endif /* ADC_MEGAXX8_H_ */
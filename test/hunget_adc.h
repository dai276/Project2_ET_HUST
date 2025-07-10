#ifndef HUNGET_ADC_H
#define HUNGET_ADC_H

#include <avr/io.h>
#include <stdint.h>

// Kh?i ADC0 (PA0), Vref=AVcc, prescaler=128
static inline void ADC_Init(void) {
	// PA0 input, pull-up off
	DDRA  &= ~(1<<PA0);
	PORTA &= ~(1<<PA0);
	// Vref=AVcc, channel=0, right adjust
	ADMUX  = (1<<REFS0);
	// Enable ADC, prescaler=128
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}

// ??c ADC0: clear MUX, start, poll, return 10-bit result
static inline uint16_t ADC_ReadChannel(uint8_t ch) {
	// gán ADMUX = Vref + channel
	ADMUX = (ADMUX & 0xF0) | (ch & 0x0F);
	ADCSRA |= (1<<ADSC);
	while (ADCSRA & (1<<ADSC));
	return ADCW;
}

#endif // HUNGET_ADC_H
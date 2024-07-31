#include "adc.h"
#include <avr/io.h>
#include "../config.h"
/*-----------------------------------------------------------------
 * ADC Voltage Readings
 * Ports PC3 (Coil A), PC4 (Coil B), PC5 (Extra), PC2 (Current Sensor)
 *-----------------------------------------------------------------*/

/*
* init_adc
* This function enables ADC capabilities on the ATMega
*/
void init_adc(void) {
	ADMUX = ADC_REF_AVCC;
	ADCSRA = ADC_ENABLE | ADC_PRESCALER_128;
}

/* read_adc
 * Reads an ADC value from a specified channel.
 * Parameters: channel - ADC channel to read
 * Returns: ADC conversion result as a uint16_t
 */
u16 read_adc(u8 channel){
	ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
	ADCSRA |= (1<<ADSC);
	while (ADCSRA & (1<<ADSC));
	return ADC;
}
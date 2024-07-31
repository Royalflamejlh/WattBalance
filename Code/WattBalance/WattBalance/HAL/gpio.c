#include "gpio.h"
#include <avr/io.h>
#include <stddef.h>
/*-----------------------------------------------------------------
 * Indicator LED Control
 * Ports PB2 (Booted), PD2 (Calibrated)
 *-----------------------------------------------------------------*/
void init_led(void){
	DDRB |= (1 << PORTB2); // Enable B2 for writing.
	DDRD |= (1 << PORTD2); // Enable D2 for writing.
}

void set_boot_led(u8 state){
	if(state){
		PORTB |= (1 << PORTB2); // Set PB2 high.
	} else{
		PORTB &= ~(1 << PORTB2); // Set PB2 low.
	}
}

void set_calibrate_led(u8 state){
	if(state){
		PORTD |= (1 << PORTD2); // Set PB2 high.
		} else{
		PORTD &= ~(1 << PORTD2); // Set PB2 low.
	}
}

/*-----------------------------------------------------------------
 * Switch Mode Control
 * Port PD3
 *-----------------------------------------------------------------*/
void init_switch(){
	DDRD &= ~(1 << PORTD3);  // Enable PD3 as an input.
	PORTD |= (1 << PORTD3);  // Enable pull-up resistor on PD3.
	
	// Configure INT1 to trigger on any logical change
	EICRA |= (1 << ISC10); 
	EICRA &= ~(1 << ISC11);

	// Enable INT1
	EIMSK |= (1 << INT1);
}

u8 get_switch(void){
	return !(PIND & (1 << PORTD3));  // Return 1 if switch is pressed, 0 otherwise
}

#include "h_bridge.h"
#include <avr/io.h>
#include <math.h>
#include "../config.h"
#include "../timer.h"

/*-----------------------------------------------------------------
 * PWM Half Bridge Control (DRV8848)
 * Ports PD4 (H_FAULT), PD5 (H_SLEEP), PD6 (PWMA), PD7 (PWMB)
 *-----------------------------------------------------------------*/

static volatile u8 duty_cycle = 0;
static volatile u8 current_tick = 0;

void pwm_callback(){
	if (current_tick < duty_cycle) {
		PORTD |= (1 << PORTD6) | (1 << PORTD7);  // Set PD6 and PD7 high.
	} else {
		PORTD &= ~((1 << PORTD6) | (1 << PORTD7));  // Set PD6 and PD7 low.
	}
	current_tick = (current_tick + 1) % DUTY_CYCLE_LENGTH;
}

void set_pwm(u16 new_duty_cycle) {
	duty_cycle = (new_duty_cycle * DUTY_CYCLE_LENGTH) / 100; // Turn percentage into a tick
}

void init_hb(void){
	current_tick = 0; // Reset current tick value.
	
	DDRD |= (1 << PORTD6) | (1 << PORTD7); // Setup D6 and D7 for Output.
	
	DDRD  |= (1 << PORTD4);  // Enable D4 for output.
	PORTD &= ~(1 << PORTD4); // Make D4 active low.
	
	DDRD &= ~(1 << PORTD5);  // Enable PD5 as an input.
	PORTD |= (1 << PORTD5);  // Enable pull-up resistor on PD5.
	
	PORTD |= (1 << PORTD4);  // Enable chip setting sleep high.
	
}

void start_pwm(){
	current_tick = 0; // Reset current tick value.
	init_timer2(HB_TIMER, pwm_callback); // Start the timer
}

void stop_pwm(){
	stop_timer2(); // Stop the timer to stop the output
	current_tick = 0; // Reset current tick value.
	PORTD &= ~((1 << PORTD6) | (1 << PORTD7));  // Set PD6 and PD7 low.
}
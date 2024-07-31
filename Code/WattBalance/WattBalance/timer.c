#include "timer.h"
#include "config.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stddef.h>

/*-----------------------------------------------------------------
 * Timer 1       Force/Velocity Mode Update Timer
 *-----------------------------------------------------------------*/

volatile TimerCallback timer1_function = NULL;

void init_timer1(u16 period_ms, TimerCallback callback) {
	timer1_function = callback;
	
	TCCR1B |= (1 << WGM12);  // Configure timer 1 for CTC mode.
	TIMSK1 |= (1 << OCIE1A); // Enable CTC interrupt.
	OCR1A = (F_CPU / 64) * period_ms / 1000 - 1;
	TCCR1B |= (1 << CS11) | (1 << CS10);  
	sei();                  // Enable interrupts.
}

void set_period_timer1(u16 period_ms) {
	OCR1A = (F_CPU / 64) * period_ms / 1000 - 1;
}

void set_callback_timer1(TimerCallback callback) {
	timer1_function = callback;
}

void stop_timer1(void){
	TCCR1B &= ~((1 << CS11) | (1 << CS10)); 
}

ISR(TIMER1_COMPA_vect) {
	if(timer1_function != NULL) timer1_function();
}

/*-----------------------------------------------------------------
 * Timer 2       PWM Timer
 *-----------------------------------------------------------------*/

volatile TimerCallback timer2_function = NULL;

void init_timer2(u16 period_ms, TimerCallback callback) {
	timer2_function = callback;
	
	TCCR2A |= (1 << WGM21);  // Configure timer 2 for CTC mode
	TIMSK2 |= (1 << OCIE2A); // Enable CTC interrupt
	OCR2A = (F_CPU / 64) * period_ms / 1000 - 1;
	TCCR2B |= (1 << CS21) | (1 << CS20);  // Set prescaler to 64
	sei();                  // Enable interrupts
}

void set_period_timer2(u16 period_ms) {
	OCR2A = (F_CPU / 64) * period_ms / 1000 - 1;
}

void set_callback_timer2(TimerCallback callback) {
	timer2_function = callback;
}

void stop_timer2(void){
	TCCR2B &= ~((1 << CS21) | (1 << CS20));
}

ISR(TIMER2_COMPA_vect) {
	if(timer2_function != NULL) timer2_function();
}

/*-----------------------------------------------------------------
 * Timer 3
 *-----------------------------------------------------------------*/

volatile TimerCallback timer3_function = NULL;

void init_timer3(uint16_t period_ms, TimerCallback callback) {
	timer3_function = callback;
	
	TCCR3A = 0;
	TCCR3B = (1 << WGM32);
	TIMSK3 |= (1 << OCIE3A);
	OCR3A = (F_CPU / 64) * period_ms / 1000 - 1;
	TCCR3B |= (1 << CS31) | (1 << CS30);
	sei();
}

void set_period_timer3(uint16_t period_ms) {
	OCR3A = (F_CPU / 64) * period_ms / 1000 - 1;
}

void set_callback_timer3(TimerCallback callback) {
	timer3_function = callback;
}

void stop_timer3(void){
	TCCR3B &= ~((1 << CS31) | (1 << CS30)); 
}

ISR(TIMER3_COMPA_vect) {
	if (timer3_function != NULL) {
		timer3_function();
	}
}
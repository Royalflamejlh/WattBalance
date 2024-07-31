#include "display.h"
#include "../lib/tm1637/tm1637.h"
#include "../config.h"
#include "../types.h"

/*-----------------------------------------------------------------
 * 4x7 Segment LED Display Control (Tm1637)
 * Ports PB0 (Dio), PB1 (CLK)
 *-----------------------------------------------------------------*/
void init_display(void){
	TM1637_init(HIGH, 7); // Set the Display to on at the max brightness.
	TM1637_enable(HIGH); // Make sure the display is on.
	TM1637_display_colon(HIGH); // Turn on the colon.
}

void set_value_display(double value){
	TM1637_clear();
	
	int displayValue = (int)(value * 100 + 0.5);
	TM1637_display_digit(3, displayValue % 10);
	
	displayValue /= 10;
	TM1637_display_digit(2, displayValue % 10 | 0x80);
	
	displayValue /= 10;
	TM1637_display_digit(1, displayValue % 10);
	
	displayValue /= 10;
	TM1637_display_digit(0, displayValue % 10);
	
	TM1637_display_colon(HIGH); // Turn on the colon.
}
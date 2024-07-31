#include "dac.h"
#include "../timer.h"
#include "../types.h"
#include "../config.h"
#include "../lib/ic2master/i2cmaster.h"
#include <math.h>
#include <avr/io.h>

/*-----------------------------------------------------------------
 * DAC Control (MCP4725A2T-E/CH)
 * Ports PE0 (SDA), PE1 (SCL)
 *------------------------------------------------------------------*/
volatile int wave_index = 0; // Stores where in the wave we are.
u16 sine_wave[MAX_SAMPLES_PER_CYCLE]; // Wave table.
u8 sin_samples = 0; // How many samples are in the wave table
u16 dac_timer_period = 0; // The period for the dac

static void compute_sine_wave(u16 samples) {
	for (int i = 0; i < samples; i++) {
		double value = sin((2 * PI * i) / samples);
		sine_wave[i] = (u16)((value + 1) * 2047.5) & 0x0FFF; // Shift and scale to 12-bit range
	}
}

void set_period_dac(u16 period){ //period for each full wave in ms
	dac_timer_period = period / sin_samples;
	set_period_timer3(dac_timer_period);
}

void send_dac_value(uint16_t dac_value) {
	uint8_t command[4];
	command[0] = 0b01000000; //Write command
	
	dac_value &= 0x0FFF;
	command[1] = dac_value >> 4;
	command[2] = (dac_value & 0x000F) << 4;
	i2c_start(DAC_ADDRESS+I2C_WRITE);
	i2c_write(command[0]); 
	i2c_write(command[1]); 
	i2c_stop();
}

void dac_callback(){
	send_dac_value(sine_wave[wave_index]);
	wave_index = (wave_index + 1) % sin_samples;
}
	
void start_dac(void){
	init_timer3(dac_timer_period, dac_callback);
}

void stop_dac(void){
	stop_timer3();
	send_dac_value(DAC_IDLE_VALUE);
}

void init_dac(u8 samples){
	compute_sine_wave(samples);
	sin_samples = samples;
	i2c_init();
	send_dac_value(DAC_IDLE_VALUE);
}

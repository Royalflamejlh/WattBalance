#include "config.h"
#include "types.h"

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <math.h>

#include "HAL/adc.h"
#include "HAL/uart.h"
#include "HAL/h_bridge.h"
#include "HAL/dac.h"
#include "HAL/gpio.h"
#include "HAL/display.h"
#include "HAL/spi_he.h"

#include "sensors.h"
#include "timer.h"

//Tuning Parameters
static double v_period = VELOCITY_PERIOD;
static u16 dac_period = DAC_WAVE_PERIOD;
static double v_min = MIN_VELOCITY;
static u16 v_calibrate_samples = CALIBRATE_VEL_SAMPLES;
static u16 v_max_samples = MAX_VEL_SAMPLES;
static u16 pos_error = POSITION_ERROR;
static u16 f_period = FORCE_PERIOD;
static double g = GRAVITY;
static u16 cycle_param = CYCLE_PARAMETER;
static double mass_offset = MASS_OFFSET_PARAM;


/*-----------------------------------------------------------------
 * Velocity Mode Measurement
 * Runs the Scale in Velocity Mode
 *-----------------------------------------------------------------*/
static volatile double vel_accumulated_blv = 0;
static volatile u16 vel_sample_count = 0;
static volatile i16 last_pos = 0;


void velocity_update(){
	double volt_a = get_voltage_coila();
	i16 pos = get_position();
	double vel = (pos - last_pos) / (v_period / 1000.0);
	if (fabs(vel) > v_min) {
		vel_accumulated_blv += volt_a / vel;
		vel_sample_count++;
	}
	last_pos = pos;
	
	// Reset accumulation after reaching max samples
	if (vel_sample_count >= v_max_samples) {
		vel_accumulated_blv = 0;
		vel_sample_count = 0;
		set_calibrate_led(LOW);
	}
	
	// Set calibrated indicator once enough samples have been read
	if( vel_sample_count >= v_calibrate_samples){
		set_calibrate_led(HIGH);
	}
	
}

int velocity_mode_start(void){
	set_calibrate_led(LOW);
	vel_sample_count = 0;
	vel_accumulated_blv = 0;
	last_pos = 0;
	set_period_timer1(v_period);
	set_callback_timer1(velocity_update);
	set_period_dac(dac_period);
	start_dac();
	return 0;
}

int velocity_mode_stop(void){
	stop_timer1();
	stop_dac();
	return 0;
}

/*-----------------------------------------------------------------
 * Force Mode Measurement
 * Runs the Scale in Force Mode
 *-----------------------------------------------------------------*/
static volatile u8 duty_cycle = 0;
static volatile double mass = 0;

double calculate_mass(double current, double blv){
	return ((current * blv ) / g) + mass_offset;
}

void force_update(void){
	i16 pos = get_position();
	if(abs(pos) <= pos_error){
		if(vel_sample_count <= 1) return;
		double blv = vel_accumulated_blv / vel_sample_count;
		mass = calculate_mass(get_current_coila(), blv);
		set_value_display(mass);
	}
	
	u16 duty_cycle_new = duty_cycle + (cycle_param * pos);
	
	set_pwm(duty_cycle_new);
}

int force_mode_start(void){
	start_pwm(); // Enable the PWM output of the h-bridge.
	set_period_timer1(f_period);
	set_callback_timer1(force_update);
	return 0;
}

int force_mode_stop(void){
	stop_timer1();
	stop_pwm();
	return 0;
}

/*-----------------------------------------------------------------
 * Switching Function
 *-----------------------------------------------------------------*/

void update_mode(){
	// Check if mode has been updated in last 10ms
	if(get_switch()){
		velocity_mode_stop();
		force_mode_start();
	}
	else{
		force_mode_stop();
		velocity_mode_start();
	}
}

/*-----------------------------------------------------------------
 * Main Control Loop
 *-----------------------------------------------------------------*/

int main(void)
{
	//Boot Up
	init_adc();
	init_dac(SAMPLES_PER_CYCLE);
	init_display();
	init_hb();
	init_spi_he();
	init_led();
	init_switch();
	init_uart(BAUD);
	
	set_boot_led(HIGH);
	set_calibrate_led(LOW);
	
	while (1)
	{
		update_mode();
		_delay_ms(MAIN_LOOP_SPEED);
	}
	return 0;
}

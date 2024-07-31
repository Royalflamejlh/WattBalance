#ifndef CONFIG_H
#define CONFIG_H

#define F_CPU 16000000UL  // System Clock Frequency

#define BAUD 9600 // UART Baud rate

//#define USE_HARDWARE_I2C 1

//Basic Stuff
#define HIGH 1
#define LOW  0
#define MAIN_LOOP_SPEED 100 // Delay to update the switch mode and send uart data (ms)

#define VELOCITY_PERIOD 1 // In ms
#define FORCE_PERIOD 5    // In ms

#define MASS_OFFSET_PARAM 0 // Added / Subtracted onto mass

// Velocity Sampling
#define MIN_VELOCITY 0.01
#define MAX_VEL_SAMPLES 5000 // Max amount of samples before a reset
#define CALIBRATE_VEL_SAMPLES 500 // How many samples until its "calibrated"

// Definitions for ADC
#define ADC_REF_AVCC (1<<REFS0)											//ADC Reference Vcc (3.3v)
#define ADC_ENABLE (1<<ADEN)											//Enable ADC Value
#define ADC_PRESCALER_128 ((1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0))		//(16 MHz / 128 = 125 KHz)
#define CURRENT_ADC 2													//Channel #
#define COIL_A_ADC  3													//Channel #
#define COIL_B_ADC  4													//Channel #
#define EXTRA_ADC   5													//Channel #

//Tuning Values
#define GRAVITY 9.81
#define V_REF 3.3
#define V_BIAS_A 1.65
#define V_BIAS_B 1.65
#define V_BIAS_COILA (1.65/3.3) * 1023
#define V_BIAS_COILB (1.65/3.3) * 1023
#define V_BIAS_EXT   (1.65/3.3) * 1023

//Hall Effect Values
#define POSITION_ERROR 100

//Half Bridge
#define CYCLE_PARAMETER -0.1
#define HB_TIMER           .1 //ms
#define DUTY_CYCLE_LENGTH  256

//DAC / Sine Wave
#define DAC_ADDRESS 0xC8
#define PI 3.14159265358979323846
#define SAMPLES_PER_CYCLE 100
#define MAX_SAMPLES_PER_CYCLE 100
#define DAC_WAVE_PERIOD  1000 //1 second per wave
#define DAC_IDLE_VALUE   2047 // Make DAC idle at 2047 so the opamp is outputting 0v


//Current Sensor Specs
#define V_ZERO_CURRENT 1.5 // Voltage at 0 Amps
#define V_CURRENT_SENS 0.4 // V/A

//Flags
#define NO_MODE 0
#define F_MODE  1
#define V_MODE  2

#endif // CONFIG_H

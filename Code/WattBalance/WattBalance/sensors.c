#include "sensors.h"
#include "HAL/adc.h"
#include "config.h"

/*-----------------------------------------------------------------
 * Get Sensor Readings
 *-----------------------------------------------------------------*/

double get_voltage_coila(void){
	u16 adc_val = read_adc(COIL_A_ADC);
	double voltage = (((double)adc_val * V_REF)/ 1024.0)  - V_BIAS_A;
	return voltage;
}

double get_voltage_coilb(void){
	u16 adc_val = read_adc(COIL_B_ADC);
	double voltage = (((double)adc_val * V_REF)/ 1024.0)  - V_BIAS_B;
	return voltage;
}

double get_current_coila(void){
	u16 adc_val = read_adc(CURRENT_ADC);
	double current = ((((double)adc_val * V_REF)/ 1024.0)  - V_ZERO_CURRENT) / V_CURRENT_SENS; //(Volts - 1.5) / (V/A)
	return current;
}

i16 get_position(void){
	return 0;
}
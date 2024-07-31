#ifndef TYPES_H
#define TYPES_H
//Macros
#include <avr/io.h>
typedef int8_t   i8;
typedef uint8_t  u8;
typedef	int16_t	 i16;
typedef uint16_t u16;
typedef	int32_t	 i32;
typedef	uint32_t u32;
typedef	int64_t	 i64;
typedef	uint64_t u64;
//Float is 4 bits
//Double is 8 bits

typedef void (*TimerCallback)(void);

typedef struct {
    uint8_t param_code;
    double v_period;
    uint16_t dac_period;
    double v_min;
    uint16_t v_calibrate_samples;
    uint16_t v_max_samples;
    uint16_t pos_error;
    uint16_t f_period;
    double g;
    uint16_t cycle_param;
    double mass_offset;
} ParameterData;

typedef struct {
    uint8_t sensor_code;
    double voltage_coil_A;
    double voltage_coil_B;
    double current_coil_A;
    int16_t position;
    double calculated_mass;
} SensorData;


#endif //TYPES_H
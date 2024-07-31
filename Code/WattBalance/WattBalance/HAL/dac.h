#ifndef DAC_H
#define DAC_H
#include "../types.h"

void init_dac(u8 samples);
void set_period_dac(u16 period); //period for each full sine wave in ms
void start_dac(void);
void stop_dac(void);
#endif // DAC_H
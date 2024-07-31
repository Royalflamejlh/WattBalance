#ifndef ADC_H
#define ADC_H

#include "../types.h"

void init_adc(void);
u16 read_adc(u8 channel);

#endif //ADC_H
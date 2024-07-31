#ifndef H_BRIDGE_H
#define H_BRIDGE_H

#include "../types.h"
void init_hb(void);

void set_pwm(u16 duty_cycle);
void start_pwm();
void stop_pwm();
#endif // H_BRIDGE_H
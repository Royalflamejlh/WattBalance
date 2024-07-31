#ifndef GPIO_H
#define GPIO_H

#include "../types.h"

void init_led(void);
void set_boot_led(u8 state);
void set_calibrate_led(u8 state);

void init_switch();
u8 get_switch(void);

#endif // GPIO_H
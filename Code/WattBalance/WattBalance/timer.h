#ifndef TIMER_H
#define TIMER_H

#include "types.h"

void init_timer1(u16 period_ms, TimerCallback callback);
void set_period_timer1(u16 period_ms);
void stop_timer1(void);
void set_callback_timer1(TimerCallback callback);

void init_timer2(u16 period_ms, TimerCallback callback);
void set_period_timer2(u16 period_ms);
void set_callback_timer2(TimerCallback callback);
void stop_timer2(void);

void init_timer3(u16 period_ms, TimerCallback callback);
void set_period_timer3(u16 period_ms);
void set_callback_timer3(TimerCallback callback);
void stop_timer3(void);

#endif // TIMER_H
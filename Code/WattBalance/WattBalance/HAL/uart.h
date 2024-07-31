#ifndef UART_H
#define UART_H

#include "../types.h"
void init_uart(u16 baud);
void transmit_uart(u8 data);
u8 receive_uart(void);

#endif //UART_H
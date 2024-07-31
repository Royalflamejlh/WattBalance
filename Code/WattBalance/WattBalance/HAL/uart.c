#include <avr/io.h>
#include "uart.h"
#include "../config.h"
/*-----------------------------------------------------------------
 * UART Communication (FT232RL)
 * Ports PD0 (ATMega RX), PD1 (ATMega TX)
 *-----------------------------------------------------------------*/
void init_uart(u16 baud) {
    u16 ubrr_value = F_CPU / (16 * baud) - 1; // Calculate baud rate register value
    UBRR0H = (u8)(ubrr_value>>8);     // Set baud rate high byte
    UBRR0L = (u8)ubrr_value;          // Set baud rate low byte
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);              // Enable receiver and transmitter
    UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);            // Set frame format: 8 data bits, 1 stop bit
}

void transmit_uart(u8 data) {
    while (!(UCSR0A & (1<<UDRE0)));
    UDR0 = data;
}

u8 receive_uart(void) {
	while (!(UCSR0A & (1<<RXC0)));
	return UDR0;
}


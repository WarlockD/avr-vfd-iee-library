/*
 * usart.cpp
 *
 * Created: 1/16/2014 6:54:05 PM
 *  Author: Paul
 */ 
#include "usart.h"

#include <avr/io.h>
#include <util/delay.h>

// we are using a 7.3728mhz chip for easy bandrate stuff. 
// check out http://www.wormfood.net/avrbaudcalc.php for the numbers I am using
#define USART_UBBR_VALUE(BAND) ((F_CPU/(BAND<<4))-1)
#define UBRR_9600 47
#define UBRR_19200 23
#define UBRR_57600 7
#define UBRR_115200 3
#define UBRR_230400 1

usart::usart() {
	set_band(UBRR_9600);
	// Set frame format to 8 data bits, no parity, 1 stop bit
	UCSRC = (0<<USBS)|(1<<UCSZ1)|(1<<UCSZ0);
	// Enable receiver and transmitter
	UCSRB = (1<<RXEN)|(1<<TXEN);
}
usart::usart(unsigned char band) {
	set_band(band);
	// Set frame format to 8 data bits, no parity, 1 stop bit
	UCSRC = (0<<USBS)|(1<<UCSZ1)|(1<<UCSZ0);
	// Enable receiver and transmitter
	UCSRB = (1<<RXEN)|(1<<TXEN);
}
void usart::send(unsigned char c) {
	/* Wait for empty transmit buffer */
	//while ((UCSRA & (1 << TXC)) == 0);
	while (!(UCSRA & (1<<UDRE)));
	// Wait if a byte is being transmitted
	/* Put data into buffer, sends the data */
	UDR = c;
}
void usart::send(const char*str) { while(*str) send(*str++); }
unsigned char usart::blocking_recieve() {
	/* Wait for data to be received */
	//while ( !(UCSRA & (1<<RXC)) );
	while((UCSRA&(1<<RXC)) == 0);
	/* Get and return received data from buffer */
	return UDR;
	
}
void usart::set_band(unsigned char band) {
	UBRRH = 0;
	UBRRL = band;
}
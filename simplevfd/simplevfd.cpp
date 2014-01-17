/*
 * simplevfd.cpp
 *
 * Created: 1/14/2014 6:45:05 PM
 *  Author: Paul
 */ 

//#define F_CPU 7372800UL
#include <avr/io.h>
#include <util/delay.h>
#include "vfd.h"

vfd display;
const unsigned char test[] = {
	0b00000100,
	0b00001010,
	0b00010001,
	0b00001010,
	0b00000100,
	0b00011111,
	0b00011111,
};

int main(void)
{
	DDRB = 0xFF;
	DDRD = _BV(PD5) | _BV(PD4) | _BV(PD2);
//	LED_ON;
//	vfd_reset();
	display.hw_reset();
	display.send('a');
	display.send('a');
	display.send('b');
	display.load_custom(0,test);
	display.send(0xF6);
	display.send(0xF6);
	//blinkled();

    while(1)
    {
		
        //TODO:: Please write your application code 
    }
}
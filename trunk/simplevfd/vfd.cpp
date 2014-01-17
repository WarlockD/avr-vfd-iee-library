/*
 * vfd.cpp
 *
 * Created: 1/16/2014 4:15:45 PM
 *  Author: Paul
 */ 
#include "vfd.h"

#include <avr/io.h>
#include <util/delay.h>

#define DATA_OUT PORTB
#define RESET_SET PORTD |= _BV(PD5)
#define RESET_CLR PORTD &= ~(_BV(PD5))
#define WRITE_SET PORTD |= _BV(PD4)
#define WRITE_CLR PORTD &= ~(_BV(PD4))
#define IS_BUSY (_BV(PD3) & PIND)
#define LED_ON PORTD |= _BV(PD2)
#define LED_OFF PORTD &= ~(_BV(PD2))

vfd::vfd() {
	DDRB = 0xFF;
	DDRD = _BV(PD5) | _BV(PD4) | _BV(PD2);
}
void vfd::send(unsigned char c)const  {
	while(IS_BUSY);
	WRITE_CLR;
	DATA_OUT = c;
	WRITE_SET;
	_delay_us(250);
	WRITE_CLR;
}
void vfd::hw_reset()const  {
	RESET_CLR;
	_delay_us(120);
	RESET_SET;
}

// Taken from the S03601-95B-40 datasheet.  It must be the way shifts happen
// its just crazy otherwise
//#define VFD_CONVERT(a,from,to) ((((font[a]) >> (from)) & 0x01) << (to))
static unsigned char VFD_CONVERT(const unsigned char* font, unsigned char from, unsigned char to) {
	return ((*font >> from) & 0x01) << to;
	//return (*font & (1 << from)) ? (1<<to) : 0;
}
#define CM_01 font+0, 4
#define CM_02 font+0, 3
#define CM_03 font+0, 2
#define CM_04 font+0, 1
#define CM_05 font+0, 0

#define CM_06 font+1, 4
#define CM_07 font+1, 3
#define CM_08 font+1, 2
#define CM_09 font+1, 1
#define CM_10 font+1, 0

#define CM_11 font+2, 4
#define CM_12 font+2, 3
#define CM_13 font+2, 2
#define CM_14 font+2, 1
#define CM_15 font+2, 0

#define CM_16 font+3, 4
#define CM_17 font+3, 3
#define CM_18 font+3, 2
#define CM_19 font+3, 1
#define CM_20 font+3, 0

#define CM_21 font+4, 4
#define CM_22 font+4, 3
#define CM_23 font+4, 2
#define CM_24 font+4, 1
#define CM_25 font+4, 0

#define CM_26 font+5, 4
#define CM_27 font+5, 3
#define CM_28 font+5, 2
#define CM_29 font+5, 1
#define CM_30 font+5, 0

#define CM_31 font+6, 4
#define CM_32 font+6, 3
#define CM_33 font+6, 2
#define CM_34 font+6, 1
#define CM_35 font+6, 0

void vfd::load_custom(unsigned char loc, const unsigned char font[]) {
	unsigned char o;
	send(0x18);								// start custom char
	send(loc < 10 ? loc  : 0xFF- loc);		// User can say 0-9 or the actual locations F6-FF
	// Here is the insane part, the bits saved in the display are in odd ball locations so we
	// have to convert them from something reasonable.  So taking a simpe 5x7 bitmap, the first two
	// bits of each byte are dropped then its converted.  If there is a better way let me know:P
	/*
	Charter Matrix
	Bit		7	6	5	4	3	2	1	0
	Byte|---------------------------------
	0	|	X	X	X	01	02	03	04	05	
	1	|	X	X	X	06	07	08	09	10
	2	|	X	X	X	11	12	13	14	15
	3	|	X	X	X	16	17	18	19	20
	4	|	X	X	X	21	22	23	24	25
	5	|	X	X	X	26	27	28	29	30
	6	|	X	X	X	31	32	33	34	35
	
	Sent to display.  
		E = 0 if end of the font load
		E = 1 incrments to the next charater and reades another load 
	Bit		7	6	5	4	3	2	1	0
	Byte|---------------------------------
	3	|	X	03	17	34	X	X	X	X
	4	|	X	07	13	30	23	04	14	22
	5	|	X	11	09	26	27	08	10	29
	6	|	X	15	05	22	31	12	06	25
	7	|	X	19	01	18	35	16	02	21
	8	|	X	E	X	X	32	28	24	20
	
	*/
	o = VFD_CONVERT(CM_03,6) | VFD_CONVERT(CM_17,5) | VFD_CONVERT(CM_34,4);
	send(o);							// send byte 3
	//4	|	X	07	13	30	23	04	14	22
	o = VFD_CONVERT(CM_07,6) | VFD_CONVERT(CM_13,5) | VFD_CONVERT(CM_30,4) | VFD_CONVERT(CM_23,3) | VFD_CONVERT(CM_04,2) | VFD_CONVERT(CM_14,1) | VFD_CONVERT(CM_33,0);
	send(o);							// send byte 4
	o = VFD_CONVERT(CM_11,6) | VFD_CONVERT(CM_09,5) | VFD_CONVERT(CM_26,4) | VFD_CONVERT(CM_27,3) | VFD_CONVERT(CM_08,2) | VFD_CONVERT(CM_10,1) | VFD_CONVERT(CM_29,0);
	send(o);							// send byte 5
	o = VFD_CONVERT(CM_15,6) | VFD_CONVERT(CM_05,5) | VFD_CONVERT(CM_22,4) | VFD_CONVERT(CM_31,3) | VFD_CONVERT(CM_12,2) | VFD_CONVERT(CM_06,1) | VFD_CONVERT(CM_25,0);
	send(o);							// send byte 6
	o = VFD_CONVERT(CM_19,6) | VFD_CONVERT(CM_01,5) | VFD_CONVERT(CM_18,4) | VFD_CONVERT(CM_35,3) | VFD_CONVERT(CM_16,2) | VFD_CONVERT(CM_02,1) | VFD_CONVERT(CM_21,0);
	send(o);							// send byte 7
	o = VFD_CONVERT(CM_32,3) | VFD_CONVERT(CM_28,2) | VFD_CONVERT(CM_24,1) | VFD_CONVERT(CM_20,0);
	o |= (1<<6);	// Set E for 1
	send(o);							// send byte 7
}
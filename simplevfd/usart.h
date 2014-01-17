/*
 * usart.h
 *
 * Created: 1/16/2014 6:53:50 PM
 *  Author: Paul
 */ 


#ifndef USART_H_
#define USART_H_

class usart {
	public:
		usart();
		usart(unsigned char);
		void send(unsigned char);
		void send(const char*);
		unsigned char blocking_recieve();
		void set_band(unsigned char);
};




#endif /* USART_H_ */
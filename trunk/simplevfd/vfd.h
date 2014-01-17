/*
 * vfd.h
 *
 * Created: 1/16/2014 4:15:35 PM
 *  Author: Paul
 */ 


#ifndef VFD_H_
#define VFD_H_


enum VFD_LEVEL {
	VFD_DIMMEST = 0x1C,
	VFD_DIM = 0x1D,
	VFD_BRIGHT = 0x1E,
	VFD_BRIGHTEST = 0x1F
};

class vfd {
	public:
		vfd();
		void send(unsigned char)const ;
		void send(const char*) const;
		void hw_reset()const ;
		inline void sw_reset() const { send(0x14); }
		inline void clear_display() const { send(0x15); }
		inline void cursor_on() const { send(0x0F); }
		inline void cursor_off() const { send(0x0E); }
		inline void cursor_home() const { send(0x16); }
		inline void cursor_left() const { send(0x08); }
		inline void cursor_right() const { send(0x09); }
		/// NORMAL DATA ENTRY WITH WRAPAROUND TO HOME POSITION (data enters beginning at the home position)
		inline void mode_normal() const { send(0x11); }
		// OVERWRITE OF RIGHT-MOST CHARACTER ON THE BOTTOM LINE ONLY/AUTOMATIC CARRIAGE RETURN OFF
		inline void mode_overwrite() const { send(0x12); }
		//	HORIZONTAL SCROLL MODE (from right to left on bottom line only, after line has been filled)
		inline void  mode_horizontal() const { send(0x13); }
		inline void brightness(VFD_LEVEL c) const { send((unsigned char)c); }
		inline void cursor_move(unsigned char loc) const { send(0x1B); send(loc & 0x3F); }
		inline void cursor_move(unsigned char x, unsigned char y) const { cursor_move(x + y *20); }
			// One or more characters may be replaced temporarily (until power-off or reset) by user-defined down-loaded character patterns.
			// Paterns are stored in F6-FF
		void load_custom(unsigned char loc, const unsigned char font[]);
};



#endif /* VFD_H_ */
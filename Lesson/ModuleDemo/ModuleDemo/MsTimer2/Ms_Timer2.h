#ifndef Ms_Timer2_h
#define Ms_Timer2_h

#include <avr/interrupt.h>

namespace MsTimer2 {
	extern unsigned long msecs;
	extern void (*func)();
	extern volatile unsigned long count;
	extern volatile char overflowing;
	extern volatile unsigned int tcnt2;
	
	void set(unsigned long ms, void (*f)());
	void start();
	void stop();
	void _overflow();
}

#endif

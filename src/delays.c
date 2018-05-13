#include "headers/project.h"

#define SYSTMR_LO        ((volatile unsigned int*)(MMIO_BASE+0x00003004))
#define SYSTMR_HI        ((volatile unsigned int*)(MMIO_BASE+0x00003008))

/**
 * Wait N CPU cycles (ARM CPU only)
 */
void wait_cycles(unsigned int number_of_cycles_to_wait)
{
	if(number_of_cycles_to_wait) while(number_of_cycles_to_wait--) 
	{
		asm volatile("nop");
	}
}

/**
 * Wait N microsec (ARM CPU only)
 */
void wait_msec(unsigned int n)
{
	register unsigned long f, t, r;
	// get the current counter frequency
	asm volatile ("mrs %0, cntfrq_el0" : "=r"(f));
	// read the current counter
	asm volatile ("mrs %0, cntpct_el0" : "=r"(t));
	// calculate expire value for counter
	t+=((f/1000)*n)/1000;
	do{asm volatile ("mrs %0, cntpct_el0" : "=r"(r));}while(r<t);
}

/**
 * Get System Timer's counter
 */
unsigned long get_system_timer()
{
	unsigned int h=-1, l;
	// we must read MMIO area as two separate 32 bit reads
	h=*SYSTMR_HI;
	l=*SYSTMR_LO;
	// we have to repeat it if high word changed during read
	if(h!=*SYSTMR_HI) {
		h=*SYSTMR_HI;
		l=*SYSTMR_LO;
	}
	// compose long int value
	return ((unsigned long) h << 32) | l;
}

/**
 * Micros is an Arduino compadible function with a resolution of 4 Mhz
 * it returns the count of microseconds since the board began running
 * it uses the BCM System Timer so it doesn't work in QEMU
 */
unsigned long micros()
{
	unsigned long current_time = get_system_timer();
	
	if(current_time != 0)
	{
	
		unsigned long current_time_modulo = current_time % 4;
		if(current_time_modulo == 0)
		{
			return current_time;
		}
		else if(current_time_modulo < 2 || current_time_modulo == 2)
		{
			return (current_time - current_time_modulo);
		}
		else
		{
			return (current_time + 1);
		}
	}
	else return 0;
}

/**
 * Wait N microsec (with BCM System Timer)
 */
void wait_msec_st(unsigned int n)
{
	unsigned long t=get_system_timer();
	// we must check if it's non-zero, because qemu does not emulate
	// system timer, and returning constant zero would mean infinite loop
	if(t) 
	{
		while(get_system_timer() < t+n);
	}
}

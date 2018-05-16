#include "headers/project.h"

#define SYSTEM_TIMER_LOW	((volatile unsigned int *)(MMIO_BASE+0x000003004))
#define SYSTEM_TIMER_HIGH	((volatile unsigned int *)(MMIO_BASE+0x000003008))

/*
 * Wait a certain number of cycles using the CPU with multiple nop commands
 * Works with QEMU
 */
void wait_cycles(unsigned int number_of_cycles_to_wait)
{
	if(number_of_cycles_to_wait) // If zero wasn't passed to the function
	{
		while(number_of_cycles_to_wait--) // If the number of cycles isn't zero decrement the
		{ 				  // number and run the loop if it is zero don't
			asm volatile ("nop");     // execute it
		}
	}
}

/*
 * Wait a certain number of microseconds using the ARM CPU
 * This is done by 
 * First getting the counter frequency
 * Then read the current counter
 * Thirdly calculate the expire value for the counter
 * Finaly in a do while loop continue reading the counter until it reaches the expire value
 */
void wait_usec(unsigned int usec)
{
	register unsigned long frequency, current_count, expire_value;	
	asm volatile ("mrs %0, cntfrq_el0" : "=r"(frequency)); // Get the frequency
	asm volatile ("mrs %0, cntpct_el0" : "=r"(expire_value)); //Get current value of counter
	expire_value += ((frequency/1000)*usec)/1000; //Calculate expire value for counter
	do
	{
		asm volatile ("mrs %0, cntpct_el0" : "=r"(current_count));
	}
	while(current_count < expire_value);
}

/*
 * Get the BCM System Timer's counter
 * This is done by
 * First do two seperate 32 bit reads of the SYSTEM_TIMER_LOW and SYSTEM_TIMER_HIGH
 * Then if the high bits changed during the LOW read then we have to read it again
 * no register will change in the time it takes to read them agian
 * Then join the two together to an unsigned long (64bit integer)
 */
unsigned long get_system_timer()
{
	unsigned int high_part, low_part;
	high_part = -1; //Only set this because it has the posibility of 
			//having 0 in the SYSTEM_TIMER_HIGH
	high_part = *SYSTEM_TIMER_HIGH;
	low_part  = *SYSTEM_TIMER_LOW;
	if(high_part != *SYSTEM_TIMER_HIGH)
	{
		high_part = *SYSTEM_TIMER_HIGH;
		low_part  = *SYSTEM_TIMER_LOW;
	}
	return ((unsigned long)high_part << 32) | low_part; // Put the value of the high_part in the 
							    // 32 most significant bits and the
							    // low_part in the 32 least significant bits
}

/*
 * Wait a certain number of microseconds using the BCM System Timer
 * To do this we have to read from the get_system_timer() function
 * Then if 0 isn't returned continue to read from the timer until
 * it reaches the sytem timer + the number of microseconds needed to 
 * wait
 * This won't work in QEMU becasue QEMU doesn't emulate the timer
 * get_system_timer() will return 0 if it cannot find the system
 * timer so this function needs to check for get_system_timer()
 * returning 0 and if it does then end this function
 */
void wait_usec_system_timer(unsigned int number_of_micros_to_wait)
{
	unsigned long current_time;
	current_time = get_system_timer();
	if(current_time)
	{
		while(get_system_timer() < number_of_micros_to_wait + current_time);
	}
}

/*
 * Micros is an Arduino compadible function with a resolution of 4Mhz 
 * it returns the count of microseconds since the board began running 
 * because it uses the BCM System Timer ti isn't compadible with QEMU
 * All this function does is get the system timer then run a modulo
 * division on the counter value with 4 and then check the output
 * and round the value to the closest multiple of four
 */
unsigned long micros()
{
	unsigned long current_time = get_system_timer();
	if(current_time)
	{
		unsigned long current_time_modulo = current_time % 4;
		if(!current_time_modulo)
		{
			return current_time;
		}
		else if(current_time_modulo <= 2)
		{
			return (current_time - current_time_modulo);
		}
		else
		{
			return (current_time + 1);
		}
	}
	return 0;
}

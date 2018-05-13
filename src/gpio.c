#include "headers/project.h"

bool gpio_setup(unsigned int gpio, GPIOMODE mode) 
{
	unsigned int* GPFSEL  = (unsigned int*) (MMIO_BASE + 0x200000 + 0x0);
	if (gpio > 54) return false;		// Check GPIO pin number valid, return false if invalid
	if (mode < 0 || mode > GPIO_ALTFUNC3) return false;	// Check requested mode is valid, return false if invalid
	unsigned int bit = ((gpio % 10) * 3);	// Create bit mask
	unsigned int mem = GPFSEL[gpio / 10];	// Read register
	mem &= ~(7 << bit);		// Clear GPIO mode bits for that port
	mem |= (mode << bit);		// Logical OR GPIO mode bits
	GPFSEL[gpio / 10] = mem;	 // Write value to register
	return true;	// Return true
}

bool gpio_input(unsigned int gpio) 
{
	unsigned int* GPLEV  = (unsigned int*) (MMIO_BASE + 0x200000 + 0x34);
	if (gpio < 54)	// Check GPIO pin number valid, return false if invalid
	{
		unsigned int bit = 1 << (gpio % 32);	// Create mask bit
		unsigned int  mem = GPLEV[gpio / 32];	// Read port level
		if (mem & bit) return true;	// Return true if bit set
	}
	return false;	// Return false
}

bool gpio_output(unsigned int gpio, bool on)
{
	if (gpio < 54)  // Check GPIO pin number valid
	{
				volatile unsigned int* p;                             // Create temp pointer
		unsigned int regnum = gpio / 32;                      // Set or Clear Register number (0 or 1)
		unsigned int bit = 1 << (gpio % 32);                  // Create mask bit
		if (on) p =  (unsigned int*)(GPSET0);		      // On == true means ptr p to set register
		else p = (unsigned int*)(GPCLR0);		      // On == false means ptr p to clear register
		p[regnum] = bit;	                                      // Output bit mask to register selected
		return true;                                          // Return true on success 
	}
	return false;                                                 // Return false if gpio set failed
}

void pinMode(unsigned int gpio, GPIOMODE mode)
{
	gpio_setup(gpio, mode);
}

void digitalWrite(unsigned int gpio, unsigned int value)
{
	gpio_output(gpio, value);
}

int  digitalRead(unsigned int gpio)
{
	bool temp = gpio_input(gpio);
	return temp;
}

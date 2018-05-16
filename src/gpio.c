#include "headers/project.h"

/*
 * This function is practically the same as the pinmode() function
 * on the Arduino you specify a pin number then a GPIO mode
 * the different modes are as follows:
 * GPIO_INPUT
 * GPIO_OUTPUT
 * GPIO_ALTFUNC0
 * GPIO_ALTFUNC1
 * GPIO_ALTFUNC2
 * GPIO_ALTFUNC3
 * GPIO_ALTFUNC4
 * GPIO_ALTFUNC5
 *
 * The mode with the largest binary value is ALTFUNC3. That is why 
 * we check is the mode is invalid by checking if the input is larger
 * than it
 *
 * A description of this function is as follows
 * one GPFSEL register controls multiple pins depending on what bits are written to 
 * in the register. 
 * 1. Get the address of the first GPFSEL register
 * 2. Check inputs to see is they are valid if they aren't return false
 * 3. Create a bit mask to write to the gpio we want
 * 4. Read the register corralating with the gpio we want to set
 * 5. Clear GPIO mode bits for the pin we want to set
 * 6. Use a logical or to write to the read of the register
 * 7. Write the value from the previous instruction or the register
 * 8. Return true
 */
bool gpio_setup(unsigned int gpio_pin, GPIOMODE mode)
{
	unsigned int *GPFSEL = (unsigned int *)(MMIO_BASE + 0x200000 + 0x0); //Get register address
	if(gpio_pin > 54 || mode < 0 || mode > GPIO_ALTFUNC3) return false; //Check inputs
	unsigned int bit_mask = ((gpio_pin % 10) * 3); //Create bit mask
	unsigned int register_read = GPFSEL[gpio_pin / 10]; //Read the correct register
	register_read &= ~(7 << bit_mask); //Clear the mode bits of the pin we want to set
	register_read |= (mode << bit_mask); //Set the gpio mode
	GPFSEL[gpio_pin / 10] = register_read; // Write to the register
	return true;
}

/*
 * gpio_input(unsigned int gpio_pin)
 * This function returns whether a GPIO pin is receiving a voltage or not
 * it returns true if it is and false if it isn't
 * Only works if the pin mode is set to input
 *
 * The steps are as follows:
 * 1. Set the register address
 * 2. Check the inputs
 * 3. If the inputs are ok then do the following steps
 * 4. Create the bit mask
 * 5. Read the port level
 * 6. Return true if the bit is set
 * 7. Return false
 */

         //////////////////////////////TODO/////////////////////////
	 // Create a way of tracking the gpio mode in software and//
	 // throw an error if is isn't the right gpio mode        //
	 //////////////////////////////TODO/////////////////////////

bool gpio_input(unsigned int gpio_pin)
{
	unsigned int *GPIO_LEVEL = (unsigned int *)(MMIO_BASE + 0x200000 + 0x34);
	if(gpio_pin < 54) //The gpio_pin cannot be negative it is unsigned
	{
		unsigned int bit_to_write = 1 << (gpio_pin % 32);
		unsigned int register_read = GPIO_LEVEL[gpio_pin / 32];
		if(register_read & bit_to_write) return true; //If the bit isn't set this evaluates to 0
	}
	return false;
}

/*
 * gpio_output(unsigned int gpio_pin, bool on)
 * If the output mode is set on a pin you can send a 3.3v signal on that pin or turn a pin off
 * 
 * To acomplish this we have to:
 * 1. Check if the GPIO pin number is valid
 * 2. Create a temporary pointer
 * 3. Set which register we need to write to
 * 4. Create the bit mask
 * 5. If the output is being turned on then set the temp pointer to the GPSET0 register
 * 6. Else set the temp pointer GPCLR0 register
 * 7. Output the bitmask to the selected register
 * 8. Return true on sucsess
 * 9. Return false if the GPIO set failed
 */
bool gpio_output(unsigned int gpio_pin, bool on)
{
	if(gpio_pin < 54)
	{
		volatile unsigned int *clear_or_set_register_pointer;
		unsigned int register_number = gpio_pin / 32;
		unsigned int bit_to_write = 1 << (gpio_pin % 32);
		if(on)
		{
			clear_or_set_register_pointer = (unsigned int *)(GPSET0);
		}
		else
		{
			clear_or_set_register_pointer = (unsigned int *)(GPCLR0);
		}
		clear_or_set_register_pointer[register_number] = bit_to_write;
		return true;
	}
	return false;
}

/*
 * These following functions are just to make a mostly arduino compadible environment
 * [WARNING] These functions fail silently
 */

void pinMode(unsigned int gpio_pin, GPIOMODE mode)
{
	gpio_setup(gpio_pin, mode);
}

void digitalWrite(unsigned int gpio_pin, bool value)
{
	gpio_output(gpio_pin, value);
}

bool digitalRead(unsigned int gpio_pin)
{
	return gpio_input(gpio_pin);
}

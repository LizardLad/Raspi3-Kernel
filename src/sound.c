#include "headers/gpio.h"

int init_sound_jack()
{
	gpio_setup(40, GPIO_ALTFUNC0);	//
	gpio_setup(45, GPIO_ALTFUNC0);	//Phone Jack pins

	uint64_t *clock_manager = MMIO_BASE + 0x101000;
	uint64_t clock_manager_password = 0x5A000000;
	uint64_t *clock_manager_pwm_divisor = clock_manager + 0x0A4;
	*clock_manager_pwm_divisor = clock_manager_password | 0x2000; //Set clock block 0

	*(clock_manager + 0x0A0) = (clock_manager_password | 0x10) | (0x01 + 0x05); //Set clock block 1

	uint64_t *pulse_width_modulation_addr = MMIO_BASE + 0x20C000;
	*(pulse_width_modulation_addr + 0x10) = 0x2C48;
	*(pulse_width_modulation_addr + 0x20) = 0x2C48;

	*(pulse_width_modulation_addr + 0x0) = 0x20 + 0x100 + 0x2000 + 0x1 + 0x40;
	return 0;
}

//I really hope this works!

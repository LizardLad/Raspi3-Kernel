#include "headers/project.h"

uint32_t servo_c_servo_pin;

void setup_servo(uint32_t pin)
{
	//Do gpio setup
	servo_c_servo_pin = pin;
	gpio_setup(pin, GPIO_OUTPUT);
}
	
void move_servo(uint8_t angle)
{
	//A servo pulse is expected every 20ms or 20,000us
	uint32_t pulse_width = (angle / 90 + 0.5) * 1000;
	gpio_output(servo_c_servo_pin, true);
	wait_usec(pulse_width);
	gpio_output(servo_c_servo_pin, false);
	wait_usec(20000 - pulse_width);
}

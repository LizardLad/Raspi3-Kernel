#ifndef STEPPER_H
#define STEPPER_H

#include "delays.h"
#include "gpio.h"
#include "math.h"

typedef struct
{
	int direction;            // Direction of rotation
	unsigned long step_delay; // delay between steps, in ms, based on speed
	int number_of_steps;      // total number of steps this motor can take
	int pin_count;            // how many pins are in use.
	int step_number;          // which step the motor is on
	int motor_pin_1;
	int motor_pin_2;
	int motor_pin_3;		// 4 and 5 phase motors only
	int motor_pin_4;		// 4 and 5 phase motors only
	int motor_pin_5;        // Only 5 phase motor
	unsigned long last_step_time; // time stamp in us of when the last step was taken
}StepperMotor;

// library interface description
void Stepper2WireInit(int number_of_steps, int motor_pin_1, int motor_pin_2, StepperMotor *Stepper);
void Stepper4WireInit(int number_of_steps, int motor_pin_1, int motor_pin_2, int motor_pin_3, int motor_pin_4, StepperMotor *Stepper);
void Stepper5WireInit(int number_of_steps, int motor_pin_1, int motor_pin_2, int motor_pin_3, int motor_pin_4, int motor_pin_5, StepperMotor *Stepper);

// speed setter functions:
void setSpeed(long whatSpeed, StepperMotor *Stepper);
// mover functions:
void step(int steps_to_move, StepperMotor *Stepper);
void stepMotor(int this_step, StepperMotor *Stepper);

#endif


#ifndef STEPPER_H
#define STEPPER_H

#include "delays.h"
#include "gpio.h"
#include "math.h"

typedef struct
{
	int32_t direction;            // Direction of rotation
	uint64_t step_delay; // delay between steps, in ms, based on speed
	int32_t number_of_steps;      // total number of steps this motor can take
	int32_t pin_count;            // how many pins are in use.
	int32_t step_number;          // which step the motor is on
	int32_t motor_pin_1;
	int32_t motor_pin_2;
	int32_t motor_pin_3;		// 4 and 5 phase motors only
	int32_t motor_pin_4;		// 4 and 5 phase motors only
	int32_t motor_pin_5;        // Only 5 phase motor
	uint64_t last_step_time; // time stamp in us of when the last step was taken
}StepperMotor;

// library interface description
void Stepper2WireInit(int32_t number_of_steps, int32_t motor_pin_1, int32_t motor_pin_2, StepperMotor *Stepper);
void Stepper4WireInit(int32_t number_of_steps, int32_t motor_pin_1, int32_t motor_pin_2, int32_t motor_pin_3, int32_t motor_pin_4, StepperMotor *Stepper);
void Stepper5WireInit(int32_t number_of_steps, int32_t motor_pin_1, int32_t motor_pin_2, int32_t motor_pin_3, int32_t motor_pin_4, int32_t motor_pin_5, StepperMotor *Stepper);

// speed setter functions:
void setStepperSpeed(int64_t whatSpeed, StepperMotor *Stepper);
// mover functions:
void step(int32_t steps_to_move, StepperMotor *Stepper);
void stepMotor(int32_t this_step, StepperMotor *Stepper);

#endif


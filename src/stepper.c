#include "headers/project.h"

void Stepper2WireInit(int32_t number_of_steps, int32_t motor_pin_1, int32_t motor_pin_2, StepperMotor *Stepper)
{
  Stepper->step_number = 0;    // which step the motor is on
  Stepper->direction = 0;      // motor direction
  Stepper->last_step_time = 0; // time stamp in us of the last step taken
  Stepper->number_of_steps = number_of_steps; // total number of steps for this motor

  // Arduino pins for the motor control connection:
  Stepper->motor_pin_1 = motor_pin_1;
  Stepper->motor_pin_2 = motor_pin_2;

  // setup the pins on the microcontroller:
  pinMode(Stepper->motor_pin_1, GPIO_OUTPUT);
  pinMode(Stepper->motor_pin_2, GPIO_OUTPUT);

  // When there are only 2 pins, set the others to 0:
  Stepper->motor_pin_3 = 0;
  Stepper->motor_pin_4 = 0;
  Stepper->motor_pin_5 = 0;

  // pin_count is used by the stepMotor() method:
  Stepper->pin_count = 2;
}


/*
 *   constructor for four-pin version
 *   Sets which wires should control the motor.
 */
void Stepper4WireInit(int32_t number_of_steps, int32_t motor_pin_1, int32_t motor_pin_2, int32_t motor_pin_3, int32_t motor_pin_4, StepperMotor *Stepper)
{
  Stepper->step_number = 0;    // which step the motor is on
  Stepper->direction = 0;      // motor direction
  Stepper->last_step_time = 0; // time stamp in us of the last step taken
  Stepper->number_of_steps = number_of_steps; // total number of steps for this motor

  // Arduino pins for the motor control connection:
  Stepper->motor_pin_1 = motor_pin_1;
  Stepper->motor_pin_2 = motor_pin_2;
  Stepper->motor_pin_3 = motor_pin_3;
  Stepper->motor_pin_4 = motor_pin_4;

  // setup the pins on the microcontroller:
  pinMode(Stepper->motor_pin_1, GPIO_OUTPUT);
  pinMode(Stepper->motor_pin_2, GPIO_OUTPUT);
  pinMode(Stepper->motor_pin_3, GPIO_OUTPUT);
  pinMode(Stepper->motor_pin_4, GPIO_OUTPUT);

  // When there are 4 pins, set the others to 0:
  Stepper->motor_pin_5 = 0;

  // pin_count is used by the stepMotor() method:
  Stepper->pin_count = 4;
}

/*
 *   constructor for five phase motor with five wires
 *   Sets which wires should control the motor.
 */
void Stepper5WireInit(int32_t number_of_steps, int32_t motor_pin_1, int32_t motor_pin_2, int32_t motor_pin_3, int32_t motor_pin_4, int32_t motor_pin_5, StepperMotor *Stepper)
{
  Stepper->step_number = 0;    // which step the motor is on
  Stepper->direction = 0;      // motor direction
  Stepper->last_step_time = 0; // time stamp in us of the last step taken
  Stepper->number_of_steps = number_of_steps; // total number of steps for this motor

  // Arduino pins for the motor control connection:
  Stepper->motor_pin_1 = motor_pin_1;
  Stepper->motor_pin_2 = motor_pin_2;
  Stepper->motor_pin_3 = motor_pin_3;
  Stepper->motor_pin_4 = motor_pin_4;
  Stepper->motor_pin_5 = motor_pin_5;

  // setup the pins on the microcontroller:
  pinMode(Stepper->motor_pin_1, GPIO_OUTPUT);
  pinMode(Stepper->motor_pin_2, GPIO_OUTPUT);
  pinMode(Stepper->motor_pin_3, GPIO_OUTPUT);
  pinMode(Stepper->motor_pin_4, GPIO_OUTPUT);
  pinMode(Stepper->motor_pin_5, GPIO_OUTPUT);

  // pin_count is used by the stepStepperMotor() function:
  Stepper->pin_count = 5;
}

/*
 * Sets the speed in revs per minute
 */
void setStepperSpeed(int64_t whatSpeed, StepperMotor *Stepper)
{
  Stepper->step_delay = 60L * 1000L * 1000L / Stepper->number_of_steps / whatSpeed;
}

/*
 * Moves the motor forward or backwards.
 */
void stepStepperMotor(int32_t thisStep, StepperMotor *Stepper)
{
  if (Stepper->pin_count == 2)
  {
	switch (thisStep)
	{
	  case 0:  // 01
		digitalWrite(Stepper->motor_pin_1, LOW);
		digitalWrite(Stepper->motor_pin_2, HIGH);
	  break;
	  case 1:  // 11
		digitalWrite(Stepper->motor_pin_1, HIGH);
		digitalWrite(Stepper->motor_pin_2, HIGH);
	  break;
	  case 2:  // 10
		digitalWrite(Stepper->motor_pin_1, HIGH);
		digitalWrite(Stepper->motor_pin_2, LOW);
	  break;
	  case 3:  // 00
		digitalWrite(Stepper->motor_pin_1, LOW);
		digitalWrite(Stepper->motor_pin_2, LOW);
	  break;
	}
  }
  if (Stepper->pin_count == 4)
  {
	switch (thisStep)
	{
	  case 0:  // 1010
		digitalWrite(Stepper->motor_pin_1, HIGH);
		digitalWrite(Stepper->motor_pin_2, LOW);
		digitalWrite(Stepper->motor_pin_3, HIGH);
		digitalWrite(Stepper->motor_pin_4, LOW);
	  break;
	  case 1:  // 0110
		digitalWrite(Stepper->motor_pin_1, LOW);
		digitalWrite(Stepper->motor_pin_2, HIGH);
		digitalWrite(Stepper->motor_pin_3, HIGH);
		digitalWrite(Stepper->motor_pin_4, LOW);
	  break;
	  case 2:  //0101
		digitalWrite(Stepper->motor_pin_1, LOW);
		digitalWrite(Stepper->motor_pin_2, HIGH);
		digitalWrite(Stepper->motor_pin_3, LOW);
		digitalWrite(Stepper->motor_pin_4, HIGH);
	  break;
	  case 3:  //1001
		digitalWrite(Stepper->motor_pin_1, HIGH);
		digitalWrite(Stepper->motor_pin_2, LOW);
		digitalWrite(Stepper->motor_pin_3, LOW);
		digitalWrite(Stepper->motor_pin_4, HIGH);
	  break;
	}
  }

  if (Stepper->pin_count == 5)
  {
	switch (thisStep)
	{
	  case 0:  // 01101
		digitalWrite(Stepper->motor_pin_1, LOW);
		digitalWrite(Stepper->motor_pin_2, HIGH);
		digitalWrite(Stepper->motor_pin_3, HIGH);
		digitalWrite(Stepper->motor_pin_4, LOW);
		digitalWrite(Stepper->motor_pin_5, HIGH);
		break;
	  case 1:  // 01001
		digitalWrite(Stepper->motor_pin_1, LOW);
		digitalWrite(Stepper->motor_pin_2, HIGH);
		digitalWrite(Stepper->motor_pin_3, LOW);
		digitalWrite(Stepper->motor_pin_4, LOW);
		digitalWrite(Stepper->motor_pin_5, HIGH);
		break;
	  case 2:  // 01011
		digitalWrite(Stepper->motor_pin_1, LOW);
		digitalWrite(Stepper->motor_pin_2, HIGH);
		digitalWrite(Stepper->motor_pin_3, LOW);
		digitalWrite(Stepper->motor_pin_4, HIGH);
		digitalWrite(Stepper->motor_pin_5, HIGH);
		break;
	  case 3:  // 01010
		digitalWrite(Stepper->motor_pin_1, LOW);
		digitalWrite(Stepper->motor_pin_2, HIGH);
		digitalWrite(Stepper->motor_pin_3, LOW);
		digitalWrite(Stepper->motor_pin_4, HIGH);
		digitalWrite(Stepper->motor_pin_5, LOW);
		break;
	  case 4:  // 11010
		digitalWrite(Stepper->motor_pin_1, HIGH);
		digitalWrite(Stepper->motor_pin_2, HIGH);
		digitalWrite(Stepper->motor_pin_3, LOW);
		digitalWrite(Stepper->motor_pin_4, HIGH);
		digitalWrite(Stepper->motor_pin_5, LOW);
		break;
	  case 5:  // 10010
		digitalWrite(Stepper->motor_pin_1, HIGH);
		digitalWrite(Stepper->motor_pin_2, LOW);
		digitalWrite(Stepper->motor_pin_3, LOW);
		digitalWrite(Stepper->motor_pin_4, HIGH);
		digitalWrite(Stepper->motor_pin_5, LOW);
		break;
	  case 6:  // 10110
		digitalWrite(Stepper->motor_pin_1, HIGH);
		digitalWrite(Stepper->motor_pin_2, LOW);
		digitalWrite(Stepper->motor_pin_3, HIGH);
		digitalWrite(Stepper->motor_pin_4, HIGH);
		digitalWrite(Stepper->motor_pin_5, LOW);
		break;
	  case 7:  // 10100
		digitalWrite(Stepper->motor_pin_1, HIGH);
		digitalWrite(Stepper->motor_pin_2, LOW);
		digitalWrite(Stepper->motor_pin_3, HIGH);
		digitalWrite(Stepper->motor_pin_4, LOW);
		digitalWrite(Stepper->motor_pin_5, LOW);
		break;
	  case 8:  // 10101
		digitalWrite(Stepper->motor_pin_1, HIGH);
		digitalWrite(Stepper->motor_pin_2, LOW);
		digitalWrite(Stepper->motor_pin_3, HIGH);
		digitalWrite(Stepper->motor_pin_4, LOW);
		digitalWrite(Stepper->motor_pin_5, HIGH);
		break;
	  case 9:  // 00101
		digitalWrite(Stepper->motor_pin_1, LOW);
		digitalWrite(Stepper->motor_pin_2, LOW);
		digitalWrite(Stepper->motor_pin_3, HIGH);
		digitalWrite(Stepper->motor_pin_4, LOW);
		digitalWrite(Stepper->motor_pin_5, HIGH);
		break;
	}
  }
}


/*
 * Moves the motor steps_to_move steps.  If the number is negative,
 * the motor moves in the reverse direction.
 */
void step(int32_t steps_to_move, StepperMotor *Stepper)
{
  int32_t steps_left = abs(steps_to_move);  // how many steps to take

  // determine direction based on whether steps_to_mode is + or -:
  if (steps_to_move > 0) { Stepper->direction = 1; }
  if (steps_to_move < 0) { Stepper->direction = 0; }


  // decrement the number of steps, moving one step each time:
  while (steps_left > 0)
  {
	uint64_t now = micros();
	// move only if the appropriate delay has passed:
	if (now - Stepper->last_step_time >= Stepper->step_delay)
	{
	  // get the timeStamp of when you stepped:
	  Stepper->last_step_time = now;
	  // increment or decrement the step number,
	  // depending on direction:
	  if (Stepper->direction == 1)
	  {
		Stepper->step_number++;
		if (Stepper->step_number == Stepper->number_of_steps) {
		  Stepper->step_number = 0;
		}
	  }
	  else
	  {
		if (Stepper->step_number == 0) {
		  Stepper->step_number = Stepper->number_of_steps;
		}
		Stepper->step_number--;
	  }
	  // decrement the steps left:
	  steps_left--;
	  // step the motor to step number 0, 1, ..., {3 or 10}
	  if (Stepper->pin_count == 5)
		stepStepperMotor(Stepper->step_number % 10, Stepper);
	  else
		stepStepperMotor(Stepper->step_number % 4, Stepper);
	}
  }
}


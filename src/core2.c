#include "include/printf.h"
#include "include/gpio.h"
#include "include/mmu.h"
#include "include/stepper.h"
#include "include/multicore.h"

void core2_main()
{
	//core_ready[1] = true;
	mmu_init();

	printf("[CORE 2] [INFO] Hello From CORE 2!\n");
	
	//Core 2 will be playing with the stepper motor!
	
	printf("[CORE 2] [INFO] Setting up gpio pins 5, 6, 13, 19 as outputs\n");
	gpio_setup(17, GPIO_OUTPUT);
	gpio_setup(18, GPIO_OUTPUT);
	gpio_setup(27, GPIO_OUTPUT);
	gpio_setup(22, GPIO_OUTPUT);
	printf("[CORE 2] [INFO] Finished setting up gpio pins 5, 6, 13, 19 as outputs\n");

	printf("[CORE 2] [INFO] Setting up 4 wire stepper motor on pins 5, 6, 13, 19\n");
	StepperMotor StepperMotor1;
	Stepper4WireInit(2048, 17, 18, 27, 22, &StepperMotor1);
	printf("[CORE 2] [INFO] Finished 4 wire stepper motor setup\n");

	printf("[CORE 2] [INFO] Set stepper motor speed 8rpm\n");
	setStepperSpeed(8, &StepperMotor1);
	printf("[CORE 2] [INFO] Stepper motor speed set!\n");
		
	printf("[CORE 2] [INFO] Starting run stepper motor on pins 5, 6, 13, 19\n");

	while(1)
	{
		step(2048, &StepperMotor1);
		printf("[CORE 2] [INFO] Revolution Complete\n");
		printf("[CORE 2] [INFO] Starting reverse revolution\n");
		step(-2048, &StepperMotor1);
		printf("[CORE 2] [INFO] Reverse revolution complete\n");
		printf("[CORE 2] [INFO] Starting forwards revolution\n");
	}
}

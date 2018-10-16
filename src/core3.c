#include <stdint.h>
#include "include/servo.h"
#include "include/printf.h"
#include "include/multicore.h"
#include "include/mmu.h"

//Well use the servos

void core3_main()
{
	core_ready[2] = true;
	mmu_init();

	printf("[CORE 3] [INFO] Hello from CORE 3!\n");

	while(1)
	{
		printf("[CORE 3] [INFO] Setting up servo on pin 10\n");
		setup_servo(10);
		printf("[CORE 3] [INFO] Finished Setup of servo\n");
		printf("[CORE 3] [INFO] Centering Servo\n");

		for(uint32_t i = 0; i < 40; i++)
		{
			move_servo(90);
		}
		printf("[CORE 3] [INFO] Centerd Servo on pin 10\n");
		printf("[CORE 3] [INFO] Setting up servo for sweeping on pin 10\n");
		
		for(int16_t i = 90; i >= 0; i--)
		{
			move_servo(i);
		}

		printf("[CORE 3] [INFO] Starting servo sweep\n");
		for(int16_t i = 0; i <= 180; i++)
		{
			move_servo(i);
		}
		
		for(int16_t i = 180; i >= 0; i--)
		{
			move_servo(i);
		}
		printf("[CORE 3] [INFO] Servo sweep finished\n");
	
		printf("[CORE 3] [INFO] Setting up servo on pin 9\n");
		setup_servo(9);
		printf("[CORE 3] [INFO] Finished Setup of servo\n");
		printf("[CORE 3] [INFO] Centering Servo\n");

		for(uint32_t i = 0; i < 40; i++)
		{
			move_servo(90);
		}
		printf("[CORE 3] [INFO] Centerd Servo on pin 9\n");
		printf("[CORE 3] [INFO] Setting up servo for sweeping on pin 9\n");
		
		for(int16_t i = 90; i >= 0; i--)
		{
			move_servo(i);
		}
	
		printf("[CORE 3] [INFO] Starting servo sweep\n");
		for(int16_t i = 0; i <= 180; i++)
		{
			move_servo(i);
		}
		
		for(int16_t i = 180; i >= 0; i--)
		{
			move_servo(i);
		}
		printf("[CORE 3] [INFO] Servo sweep finished\n");	
	}		
}

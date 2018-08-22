#include "headers/project.h"

//Well use the servos

void core3_main()
{
	*core3_ready = true;
	mmu_init();

	char x = 1;

	printf("[CORE 3] [INFO] Hello from CORE 3!\n");

	printf("[CORE 3] [INFO] Setting up servo on pin x\n");
	setup_servo(x);
	printf("[CORE 3] [INFO] Finished Setup of servo\n");
	printf("[CORE 3] [INFO] Centering Servo\n");

	for(uint32_t i = 0; i < 40; i++)
	{
		move_servo(90);
	}
	printf("[CORE 3] [INFO] Centerd Servo on pin x\n");
	printf("[CORE 3] [INFO] Setting up servo for sweeping on pin x\n");
		
	for(int16_t i = 90; i >= 0; i--)
	{
		move_servo(i);
	}

	while(1)
	{
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

#include "headers/project.h"

void core1_main()
{
	*core1_ready = true;
	mmu_init();

	printf("[CORE 1] [INFO] Hello from CORE 1!\n");
	while(1)
	{
		printf("[CORE 1] [INFO] Starting flash ACT LED\n");
		for(int32_t i = 0; i < 12; i++)
		{
			set_ACT_LED(true);
			wait_usec(100000);
			set_ACT_LED(false);
			wait_usec(100000);
		}
		printf("[CORE 1] [INFO] Flashing ACT LED Done\n");
		printf("[CORE 1] [INFO] Starting RGB flash on pins 16, 20, 21\n");
		for(int32_t i = 0; i < 36; i++)
		{
			//A total loop takes 1 second
			//pin 16 is red
			//pin 20 is green
			//pin 21 is blue
			gpio_output(16, true);
			wait_usec(100000);
			gpio_output(21, false);
			wait_usec(200000);
			gpio_output(20, true);
			wait_usec(100000);
			gpio_output(16, false);
			wait_usec(200000);
			gpio_output(21, true);
			wait_usec(100000);
			gpio_output(20, false);
			wait_usec(200000);
		}
		printf("[CORE 1] [INFO] Finished RGB flash on pins 16, 20, 21\n");
	}
}

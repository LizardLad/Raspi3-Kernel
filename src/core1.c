#include "include/stdint.h"
#include "include/gpio.h"
#include "include/led.h"
#include "include/printf.h"
#include "include/multicore.h"
#include "include/mmu.h"

void core1_main()
{
	*core1_ready = true;
	mmu_init();

	printf("[CORE 1] [INFO] Hello from CORE 1!\n");

	printf("[CORE 1] [INFO] Setting up pins 23, 24, 25 as outputs\n");
	gpio_setup(23, GPIO_OUTPUT);
	gpio_setup(24, GPIO_OUTPUT);
	gpio_setup(25, GPIO_OUTPUT);
	gpio_output(23, false);
	gpio_output(24, false);
	gpio_output(25, false);
	printf("[CORE 1] [INFO] Finished setting up pins 23, 24, 25 as outputs");

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
		printf("[CORE 1] [INFO] Starting RGB flash on pins 23, 24, 25\n");
		for(int32_t i = 0; i < 36; i++)
		{
			//pin 23 is red
			//pin 24 is green
			//pin 25 is blue
			gpio_output(23, true);
			wait_usec(100000);
			gpio_output(25, false);
			wait_usec(200000);
			gpio_output(24, true);
			wait_usec(100000);
			gpio_output(23, false);
			wait_usec(200000);
			gpio_output(24, false);
			wait_usec(100000);
			gpio_output(25, true);
			wait_usec(200000);
		}
		printf("[CORE 1] [INFO] Finished RGB flash on pins 23, 24, 25\n");
	}
}

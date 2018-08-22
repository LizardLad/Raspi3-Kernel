#include "headers/project.h"

//Well use the servos

void core3_main()
{
	*core3_ready = true;
	mmu_init();

	printf("[CORE 3] [INFO] Hello from CORE 3!");

	while(1)
	{
		asm volatile ("nop");
	}
}

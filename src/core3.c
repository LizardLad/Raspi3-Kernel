#include "headers/project.h"

//Well use the servos

void core3_main()
{
	*core3_ready = true;
	mmu_init();

	while(1)
	{
		asm volatile ("nop");
	}
}

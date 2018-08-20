#include "headers/project.h"

void main()
{
	// set up serial console
	//uart_init();
	lfb_init();
	dynamic_memory_alloc_init();
	console_init();	
	clocks_init();
	start_other_3_cores();

	// echo everything back
	while(1) 
	{
		uart_send(uart_getc());
	}
}

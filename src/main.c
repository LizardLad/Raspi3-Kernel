#include "headers/project.h"

void main()
{
	// set up serial console
	//uart_init();
	lfb_init();
	dynamic_memory_alloc_init();
	console_init();	
	clocks_init();
	breakpoint;
	//PRINT_LOCK = 1;

	// echo everything back
	while(1) 
	{
		uart_send(uart_getc());
	}
}

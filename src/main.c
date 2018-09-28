#include "headers/project.h"

//static uint32_t check_hello = 0;

void main()
{
	//Setup videocore
	init_V3D();
	
	// set up serial console
	uart_init();
	lfb_init();

	dynamic_memory_alloc_init();
	console_init();	
	clocks_init();
	init_audio_jack();
	
	//Create mmu table on Core 0
	init_page_table();

	//start_other_3_cores();
	mmu_init(); //Now turn on MMU on Core 0
	*core0_ready = true;


	printf("[INFO] GPU memory split is: %d\n", get_gpu_memory_split());

	uint32_t gpu_mem_handle = lock_gpu_memory();
	render_quad(lfb_width, lfb_height, ARM_addr_to_GPU_addr((void*)(uintptr_t)lfb), gpu_mem_handle);	

	// echo everything back
	while(1) 
	{
		uart_send(uart_getc());
	}

	// Release resources
	V3D_mem_unlock(gpu_mem_handle);
	V3D_mem_free(gpu_mem_handle);
}

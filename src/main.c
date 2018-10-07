#include "include/stdint.h"
#include "include/GLES.h"
#include "include/uart.h"
#include "include/lfb.h"
#include "include/malloc.h"
#include "include/console.h"
#include "include/sound.h"	//init_audio_jack(), play_audio...
#include "include/system.h"	//clocks_init();
#include "include/mmu.h"
#include "include/printf.h"
#include "include/multicore.h"
#include "include/gpu_memory.h"
#include "include/start.h"

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

	mmu_init(); //Now turn on MMU on Core 0
	*core0_ready = true;

	multicore_init(); //Now core_execute is avalible to be run after this

//	printf("[INFO] GPU memory split is: %d\n", get_gpu_memory_split());

//	uint32_t gpu_mem_handle = lock_gpu_memory();
//	render_quad(lfb_width, lfb_height, ARM_addr_to_GPU_addr((void*)(uintptr_t)lfb), gpu_mem_handle);	
	
	INCLUDE_BINARY_FILE(believer, "src/audio/test.bin", ".rodata.believer");
	audio_start = &believer_start;
	audio_end = &believer_end;
	play_audio();

	// echo everything back
	while(1) 
	{
		uart_send(uart_getc());
	}

	// Release resources
//	V3D_mem_unlock(gpu_mem_handle);
//	V3D_mem_free(gpu_mem_handle);
}

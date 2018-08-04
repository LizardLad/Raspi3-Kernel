#include "headers/project.h"

void main()
{
	// set up serial console
	uart_init();
	lfb_init();
	//clocks_init();
	dynamic_memory_alloc_init();
	console_init();

	//printf("Hello World! %3x %s", malloc(12 * sizeof(char)), "Well hello there lllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllll");
	asm volatile("brk #0");//breakpoint;	

	//init_audio_jack();
	//play_16bit_unsigned_audio((char *)&_binary_src_audio_The_Amazons_bin_start, (char *)&_binary_src_audio_The_Amazons_bin_end);

	//if(sd_init() == SD_OK)
	//{
	//	if(fat_getpartition())
	//	{
	//		text_file_dump("LICENC~1BRO");
	//		fat_listdirectory();
	//	}
	//}

	//asm(	"mov	x1, #0xe0\n"\
	//	"mov	x2, #0x80000\n" \
	//	"str	x2, [x1]\n"\
	//	"sev");

	// echo everything back
	while(1) 
	{
		uart_send(uart_getc());
	}
}

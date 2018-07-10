#include "headers/project.h"

void main()
{
	// set up serial console
	uart_init();
	lfb_init();
	clocks_init();
	
	init_audio_jack();
	play_16bit_unsigned_audio((char *)&_binary_src_audio_The_Amazons_bin_start, (char *)&_binary_src_audio_The_Amazons_bin_end);

	if(sd_init() == SD_OK)
	{
		if(fat_getpartition())
		{
			text_file_dump("LICENC~1BRO");
		}
	}

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

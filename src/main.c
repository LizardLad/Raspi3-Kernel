#include "headers/project.h"

void main()
{
	// set up serial console
	uart_init();
	lfb_init();
	clocks_init();
	

	/*char r, g, b;
	unsigned int frame_count = 0;
	char outbuff[50];
	while(1)
	{
		for(int x = 0; x < 1024; x++)
		{
			for(int y = 0; y < 768; y++)
			{
				hsv_to_rgb((int)(x/2.8555555555555556), 1.0, (1.0 - y / 728.0), &r, &g, &b);
				lfb_draw_pixel(x, y, r, g, b);
				// This didn't quite work I'm not sure why
				// Normalise coordinates
				//double uv_x = x / 1024;
				//double uv_y = y / 768;
				//double col_r = 0.5 + 0.5 * cos(get_system_timer() + uv_x + uv_y + 0);
				//double col_g = 0.5 + 0.5 * cos(get_system_timer() + uv_x + uv_y + 2);
				//double col_b = 0.5 + 0.5 * cos(get_system_timer() + uv_x + uv_y + 4);
				//lfb_draw_pixel(x, y, (char)col_r, (char)col_g, (char)col_b);
			}
		}
		frame_count++;
		itoa(frame_count, outbuff);
		lfb_print(0, 0, outbuff);
	}*/

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

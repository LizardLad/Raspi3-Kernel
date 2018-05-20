#include "headers/project.h"

void main()
{
	// set up serial console
	//system_init();
	uart_init();
	lfb_init();
	
	gpio_setup(3, GPIO_OUTPUT);
	
	while(1)
	{
		int k = 0;
		for(int pos = 0; pos <= 180; pos++)
		{
			k = 13.333333333333 * pos;
			gpio_output(3, true);
			wait_usec(k); //Goto 0 degrees if 180 is desired use 2400us
			gpio_output(3, false);
			wait_usec(15000 - k);
			//wait_usec(15000);
		}
		int i = 0;
		for(int pos = 180; pos >= 0; pos --)
		{
			i = 13.3333333333333 * pos;
			gpio_output(3, true);
			wait_usec(i); //Goto 0 degrees if 180 is desired use 2400us
			gpio_output(3, false);
			wait_usec(15000 - i);
			//wait_usec(15000);
		}
	}

	char r, g, b;
	for(int x = 0; x < 1024; x++)
	{
		for(int y = 0; y < 768; y++)
		{
			hsv_to_rgb((int)(x/2.8555555555555556), 1.0, (1.0 - y / 728.0), &r, &g, &b);
			//lfb_draw_pixel(x, y, r, g, b);
		}
	}
	
	//unsigned int cluster = 0;

	// initialize EMMC and detect SD card type
	//if(sd_init()==SD_OK) 
	//{
			// read the master boot record and find our partition
		//if(fat_getpartition())
		//{
			// find out file in root directory entries
			//cluster=fat_getcluster("LICENC~1BRO");
			//if(cluster==0)
			//{
			//	cluster=fat_getcluster("KERNEL8 IMG");
			//}
			//if(cluster)
			//{
				// read into memory
			//	uart_dump(fat_readfile(cluster));
			//	lfb_dump(fat_readfile(cluster));
			//}
			//cluster = text_file_dump("LICENC~1BRO");
		//} 
		//else 
		//{
		//	uart_puts("FAT partition not found???\n");
		//}
	//}

	//fat_listdirectory();

	asm(	"mov	x1, #0xe0\n"\
		"mov	x2, #0x80000\n" \
		"str	x2, [x1]\n"\
		"sev");

	// echo everything back
	while(1) 
	{
		//uart_send(uart_getc());
		asm volatile ("nop");
	}
}

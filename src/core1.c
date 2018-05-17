#include "headers/project.h"

void core1_main()
{
	lfb_special_print(0, 0, "Hello World", false, 0, 255, 0, 0, 0, 0);
	for(int i = 0; i < 12; i++)
	{
		set_ACT_LED(true);
		wait_usec(100000);
		set_ACT_LED(false);
		wait_usec(100000);
	}
	lfb_print(0, 1, "Flashing Done");
	lfb_print(0, 2, "Mateo is good boi   ");
}

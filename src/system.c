#include <stdint.h>
#include "include/mbox.h"

void set_cpu_clock(uint32_t clock_rate)
{
	mailbox_tag_message(0, 5, MBOX_TAG_SET_CLKRATE, 8, 8, CLK_ARM_ID, clock_rate);
}

void get_cpu_clock()
{
	;
}

uint32_t get_cpu_max_clock()
{
	uint32_t buffer[5] = { 0 };
	if(mailbox_tag_message(buffer, 5, MBOX_TAG_GET_MAX_CLKRATE, 8, 8, CLK_ARM_ID, 0))
	{
		return buffer[4];
	}
	return 0;
}

void clocks_init()
{
	uint32_t max_clock = get_cpu_max_clock();
	set_cpu_clock(max_clock);
}

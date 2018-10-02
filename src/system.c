#include "include/stdint.h"
#include "include/mbox.h"

void set_cpu_clock(uint32_t clock_rate)
{
	mailbox_tag_message(0, 5, 0x00038002, 8, 8, 3, clock_rate);
}

void get_cpu_clock()
{
	;
}

uint32_t get_cpu_max_clock()
{
	uint32_t buffer[5] = { 0 };
	if(mailbox_tag_message(0, 5, 0x00030004, 8, 8, 3, 0))
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

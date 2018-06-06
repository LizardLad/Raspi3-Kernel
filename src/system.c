#include "headers/project.h"

void set_cpu_clock(unsigned int clock_rate)
{
	mailbox[0] = 8*4;
	mailbox[7] = 0;
	mailbox[1] = 0;
	mailbox[2] = 0x00038002;
	mailbox[3] = 8;
	mailbox[4] = 8;
	mailbox[5] = 3;
	mailbox[6] = clock_rate;
	mailbox_tag_write((unsigned int)&mailbox[0]);
}

void get_cpu_clock()
{
	;
}

unsigned int get_cpu_max_clock()
{
	mailbox[0] = 8 * 4; //TAG is 0x00030004
	mailbox[7] = 0;
	mailbox[1] = 0;
	mailbox[2] = 0x00038004;
        mailbox[3] = 8;
        mailbox[4] = 8;
        mailbox[5] = 3;
        mailbox[6] = 0;
	mailbox_tag_write((unsigned int)&mailbox[0]);
	mailbox_tag_read(0x8);

	if(mailbox[1] == 0x80000000)
	{
		return mailbox[6];
	}

	return 0; //Return 0 on error because returning non-zero is a clock rate!
}

void clocks_init()
{
	unsigned int max_clock = get_cpu_max_clock();
	set_cpu_clock(max_clock);
}

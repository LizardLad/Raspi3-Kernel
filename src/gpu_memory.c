#include "headers/project.h"

uint32_t get_gpu_memory_split()
{
	mailbox[0] = 8 * 4;
	mailbox[1] = 0;
	mailbox[2] = 0x10005;
	mailbox[3] = 8;
	mailbox[4] = 0;
	mailbox[5] = 0;
	mailbox[6] = 0;
	mailbox[7] = 0;
	if(!mailbox_call(MBOX_CH_PROP))
	{
		mailbox[6]=512*1024*1024;
	}
	return mailbox[6]; //Return the amount of GPU memory in bytes
}

uint64_t *get_gpu_memory_address_start()
{
	return (uint64_t *)((uint64_t)(0x3F000000 - get_gpu_memory_split()));
}

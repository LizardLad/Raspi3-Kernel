#include "include/stdint.h"
#include "include/mbox.h"
#include "include/GLES.h"
#include "include/printf.h"
#include "include/multicore.h"

enum {
	MEM_FLAG_DISCARDABLE = 1 << 0,	/* can be resized to 0 at any time. Use for cached data */
	MEM_FLAG_NORMAL = 0 << 2,	/* normal allocating alias. Don't use from ARM */
	MEM_FLAG_DIRECT = 1 << 2,	/* 0xC alias uncached */
	MEM_FLAG_COHERENT = 2 << 2,	/* 0x8 alias. Non-allocating in L2 but coherent */
	MEM_FLAG_L1_NONALLOCATING = (MEM_FLAG_DIRECT | MEM_FLAG_COHERENT), /* Allocating in L2 */
	MEM_FLAG_ZERO = 1 << 4,		/* initialise buffer to all zeros */
	MEM_FLAG_NO_INIT = 1 << 5,	/* don't initialise (default is initialise to all ones */
	MEM_FLAG_HINT_PERMALOCK = 1 << 6,	/* Likely to be locked for long periods of time. */
};

uint32_t get_gpu_memory_split()
{
	mailbox[0] = 8 * 4;
	mailbox[1] = 0;
	mailbox[2] = 0x10006;
	mailbox[3] = 8;
	mailbox[4] = 0;
	mailbox[5] = 0;
	mailbox[6] = 0;
	mailbox[7] = 0;
	uint32_t addr = (uint32_t)(uintptr_t)&mailbox[0];
	asm volatile ("dc civac, %0" : : "r" (addr) : "memory");
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

uint32_t lock_gpu_memory()
{
	uint32_t handle = V3D_mem_alloc(0x800000, 0x1000, MEM_FLAG_COHERENT | MEM_FLAG_ZERO);
	if(!handle)
	{
		printf("[CORE %d] [ERROR] Unable to allocate memory", get_core_id());
		return 0;
	}
	uint32_t bus_addr = V3D_mem_lock(handle);
	return bus_addr;
}

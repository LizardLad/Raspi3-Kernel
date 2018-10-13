#include "include/stdint.h"
#include "include/mbox.h"
#include "include/GLES.h"
#include "include/printf.h"
#include "include/multicore.h"

uint32_t get_gpu_memory_split()
{
	uint32_t buffer[5] = { 0 };
	//mailbox[0] = 8 * 4;
	//mailbox[1] = 0;
	//mailbox[2] = 0x10006;
	//mailbox[3] = 8;
	//mailbox[4] = 0;
	//mailbox[5] = 0;
	if(!mailbox_tag_message(buffer, 5, MBOX_TAG_GET_VC_MEMORY, 8, 0, 0, 0))
	{
		buffer[4]=512*1024*1024;
	}
	return buffer[4]; //Return the amount of GPU memory in bytes
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

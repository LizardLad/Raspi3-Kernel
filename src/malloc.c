#include <stdint.h>
#include "include/malloc.h"
#include "include/gpu_memory.h"

void dynamic_memory_alloc_init()
{
	extern unsigned char _end;
	end_of_program = &_end;
	end_of_cpu_memory = get_gpu_memory_address_start();
	total_space_avaliable = (unsigned char *)end_of_cpu_memory - end_of_program;
	chunk_size = 1024;
	number_of_chunks = total_space_avaliable / chunk_size;
	for(uint64_t i = 0; i < number_of_chunks; i++) 
	{
		allocated[i] = 0;
		connected_chunk[i] = 0;
	}

}

char *malloc(size_t amount_to_allocate)
{
	//Remember to ofset all allocations by EndOfProgram
	//Remember to check if any chunk end hits MMIO_BASE
	
	amount_to_allocate = (((amount_to_allocate + 1024 - 1) / 1024) * 1024);
	uint64_t number_of_chunks_to_allocate = amount_to_allocate / 1024;

	for(uint64_t i = 0; i < number_of_chunks; i++)
	{
		if(allocated[i] == 0)
		{
			uint8_t chunk_allocated_in_space = 0;
			for(uint64_t j = i; j < i + number_of_chunks_to_allocate; j++)
			{
				if(connected_chunk[j] == 1)
				{
					chunk_allocated_in_space = 1;
				}
			}
			if(chunk_allocated_in_space == 0)
			{
				uint64_t temp = amount_to_allocate;
				for(uint64_t j = i; j < i + number_of_chunks_to_allocate; j++)
                        	{
					connected_chunk[j] = 1;
					allocated[j] = 1; 
					if(temp == 1024)
					{
						connected_chunk[j] = 0;
					}
					temp -= 1024;
                        	}
				unsigned char *x = end_of_program + (i << 10);
				if(x[amount_to_allocate] < (uint64_t)end_of_cpu_memory)
				{
					return (char *)x;
				}
				else
				{
					return NULL;
				}
			}
		}
	}
	return NULL;
}

int free(void *p_to_free)
{
	//WARNING THIS FUNCTION FAILS SILENTLY (But it shouldn't fail XD)
	//TODO add compadibility with latest version of malloc
	uint64_t i = 0;
	i = ((p_to_free - (void *)end_of_program) >> 10);
	allocated[i] = 0;
	//We also have to find out if it is a connected chunk to do this:
	if(connected_chunk[i] == 0) return 0;
	for(; connected_chunk[i] == 1; i++)
	{
		connected_chunk[i] = 0;
		allocated[i] = 0;
	}
	i++;
	allocated[i] = 0;	
	return 0;
}

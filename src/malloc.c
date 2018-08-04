#include "headers/project.h"

void dynamic_memory_alloc_init()
{
	extern unsigned char _end;
	EndOfProgram = &_end;
	TotalSpaceAvaliable = (unsigned char *)MMIO_BASE - EndOfProgram;
	ChunkSize = 1024;
	NumberOfChunks = TotalSpaceAvaliable / ChunkSize;
	for(unsigned long i = 0; i < NumberOfChunks; i++) 
	{
		allocated[i] = 0;
		connected_chunk[i] = 0;
	}

}

char *malloc(size_t amountToAllocate)
{
	//Remember to ofset all allocations by EndOfProgram
	//Remember to check if any chunk end hits MMIO_BASE
	
	amountToAllocate = (((amountToAllocate + 1024 - 1) / 1024) * 1024);
	uint64_t number_of_chunks_to_allocate = amountToAllocate / 1024;

	for(unsigned long i = 0; i < NumberOfChunks; i++)
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
				uint64_t temp = amountToAllocate;
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
				unsigned char *x = EndOfProgram + (i << 10);
				if(x[amountToAllocate] < MMIO_BASE)
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
	unsigned long i = 0;
	i = ((p_to_free - (void *)EndOfProgram) >> 10);
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

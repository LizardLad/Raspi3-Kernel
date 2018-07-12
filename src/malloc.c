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
	}
}

char *malloc(size_t amountToAllocate)
{
	//Remember to ofset all allocations by EndOfProgram
	//Remember to check if any chunk end hits MMIO_BASE
	if(amountToAllocate > ChunkSize)
	{
		return NULL;
	}
	for(unsigned long i = 0; i < NumberOfChunks; i++)
	{
		if(allocated[i] == 0)
		{
			allocated[i] = 1;
			unsigned char *x = EndOfProgram + (i << ChunkSize);
			if(x[ChunkSize] < MMIO_BASE)
			{
				return (char *)x;
			}
			else
			{
				return NULL;
			}
		}
	}
	return NULL;
}


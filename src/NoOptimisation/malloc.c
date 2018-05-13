#include "../headers/project.h"

void dynamic_memory_alloc_init_stage_1()
{
	extern unsigned char _start, _end;
	
	StartOfProgram = &_start;
	EndOfProgram = &_end + 512;
	TotalSpaceAvaliable = (unsigned char *)MMIO_BASE - EndOfProgram;
	ChunkSize = 1000;
	NumberOfChunks = TotalSpaceAvaliable / ChunkSize;
}

void dynamic_memory_alloc_init(char allocated[])
{
	for(unsigned long i = 0; i < NumberOfChunks; i++) 
	{
		allocated[i] = 0;
	}
}

char *malloc(size_t amountToAllocate, char allocated[])
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
			unsigned char *x = EndOfProgram + i * ChunkSize;
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


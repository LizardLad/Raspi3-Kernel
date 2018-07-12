#ifndef MALLOC_H
#define MALLOC_H

#include "stdint.h"

#define NULL 0x0

#define size_t uint64_t

void dynamic_memory_alloc_init();
char *malloc(size_t amountToAllocate);
void free(char *PointerToFree);

unsigned char *EndOfProgram;
unsigned long TotalSpaceAvaliable;
unsigned long ChunkSize;
unsigned long NumberOfChunks;
char allocated[1032160]; // This is th largest it would need to be I did the math as if the kenel had 0 size

#endif

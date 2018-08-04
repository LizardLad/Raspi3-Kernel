#ifndef MALLOC_H
#define MALLOC_H

#include "stdint.h"

#define NULL 0x0

#define size_t uint64_t

void dynamic_memory_alloc_init();
char *malloc(size_t amountToAllocate);
int free(void *p_to_free);

unsigned char *EndOfProgram;
unsigned long TotalSpaceAvaliable;
unsigned long ChunkSize;
unsigned long NumberOfChunks;
char allocated[1032160]; // This is the largest it would need to be I did the math as if the kenel had 0 byte size
char connected_chunk[1032160]; // This is the largest it would need to be I did the math as if the kenel had 0 byte size

#endif

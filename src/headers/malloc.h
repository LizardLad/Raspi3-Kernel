#ifndef MALLOC_H
#define MALLOC_H

#define NULL 0x0

#define size_t long int

void dynamic_memory_alloc_init_stage_1();
void dynamic_memory_alloc_init(char allocated[]);
char *malloc(size_t amountToAllocate, char allocated[]);
void free(char *PointerToFree);

unsigned char *StartOfProgram;
unsigned char *EndOfProgram;
unsigned long TotalSpaceAvaliable;
unsigned long ChunkSize;
unsigned long NumberOfChunks;

#endif

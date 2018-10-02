#ifndef MALLOC_H
#define MALLOC_H

#include "stdint.h"

#define NULL 0x0

#define size_t uint64_t

void dynamic_memory_alloc_init();
char *malloc(size_t amount_to_allocate);
int32_t free(void *p_to_free);
uint64_t *end_of_cpu_memory;
unsigned char *end_of_program;
uint64_t total_space_avaliable;
uint64_t chunk_size;
uint64_t number_of_chunks;
char allocated[1032160]; // This is the largest it would need to be I did the math as if the kenel had 0 byte size
char connected_chunk[1032160]; // This is the largest it would need to be I did the math as if the kenel had 0 byte size

#endif

#ifndef GPU_MEMORY_H
#define GPU_MEMORY_H

uint32_t get_gpu_memory_split();
uint64_t *get_gpu_memory_address_start();
uint32_t lock_gpu_memory();

#endif

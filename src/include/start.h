#ifndef START_H
#define START_H

/* ARM bus address to GPU bus address */
extern uint32_t ARM_addr_to_GPU_addr (void* ARMaddress);

/* GPU bus address to ARM bus address */
extern uint32_t GPU_addr_to_ARM_addr (uint32_t GPUaddress);

#endif

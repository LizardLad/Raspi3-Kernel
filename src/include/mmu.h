#ifndef _MMU_
#define _MMU_

#ifdef __cplusplus	// If we are including to a C++
extern "C" {		// Put extern C directive wrapper around
#endif
#include <stdint.h>	// Needed for uint8_t, uint32_t, etc
#include <stdbool.h>

#define MT_DEVICE_nGnRnE	0
#define MT_DEVICE_nGnRE		1
#define MT_DEVICE_GRE		2
#define MT_NORMAL_NC		3
#define MT_NORMAL		4
#define MT_NORMAL_WT		5

// MACRO TO CREATE MAIR_BITS
#define MAIR(attr, mt) ((uint64_t)attr << ((mt) * 8))

#define MAIR_VALUE  (MAIR(0x00, MT_DEVICE_nGnRnE) | \
					 MAIR(0x04, MT_DEVICE_nGnRE) | \
					 MAIR(0x0c, MT_DEVICE_GRE) | \
					 MAIR(0x44, MT_NORMAL_NC) | \
					 MAIR(0xff, MT_NORMAL) | \
					 MAIR(0xbb, MT_NORMAL_WT))

extern volatile bool core_mmu_online[4];

void init_page_table (void);

void mmu_init (void);
uint64_t virtualmap (uint32_t phys_addr, uint8_t memattrs);


extern void semaphore_inc(uint32_t* sem);	// Code is in start.S
extern void semaphore_dec(uint32_t* sem);	// Code is in start.S

#ifdef __cplusplus								// If we are including to a C++ file
}												// Close the extern C directive wrapper
#endif

#endif

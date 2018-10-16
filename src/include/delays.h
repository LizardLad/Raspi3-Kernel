#ifndef DELAYS_H
#define DELAYS

#include <stdint.h>

void wait_cycles(uint32_t number_of_cycles_to_wait);
void wait_usec(uint32_t n);
uint64_t get_system_timer();
void wait_usec_st(uint32_t n);
uint64_t micros();

#endif

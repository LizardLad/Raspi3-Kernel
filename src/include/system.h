#ifndef SYSTEM_H
#define SYSTEM_H

void get_cpu_clock();
uint32_t get_cpu_max_clock();
void set_cpu_clock(uint32_t clock_rate);
void clocks_init();

#endif

#ifndef SYSTEM_H
#define SYSTEM_H

void get_cpu_clock();
unsigned int get_cpu_max_clock();
void set_cpu_clock(unsigned int clock_rate);
void clocks_init();

#endif

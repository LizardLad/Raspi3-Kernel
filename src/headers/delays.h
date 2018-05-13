#ifndef DELAYS_H
#define DELAYS

void wait_cycles(unsigned int n);
void wait_msec(unsigned int n);
unsigned long get_system_timer();
void wait_msec_st(unsigned int n);
unsigned long micros();

#endif

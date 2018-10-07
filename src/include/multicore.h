#ifndef MULTICORE_H
#define MULTICORE_H

#include "stdbool.h"

extern volatile bool *core0_ready;
extern volatile bool *core1_ready;
extern volatile bool *core2_ready;
extern volatile bool *core3_ready;

void multicore_init();
int get_core_id();

int core_execute(char core_id, void (*func_ptr) (void *), void *data_ptr);

#endif

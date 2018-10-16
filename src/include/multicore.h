#ifndef MULTICORE_H
#define MULTICORE_H

#include "stdbool.h"

extern volatile bool core_ready[3];

void multicore_init();
int get_core_id();

int core_execute(char core_id, void (*func_ptr) (void *), void *data_ptr);

#endif

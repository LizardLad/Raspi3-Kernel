#ifndef MULTICORE_H
#define MULTICORE_H

#include "stdbool.h"

extern volatile bool *core0_ready;
extern volatile bool *core1_ready;
extern volatile bool *core2_ready;
extern volatile bool *core3_ready;

void start_other_3_cores();

#endif

#ifndef DEBUG_H
#define DEBUG_H

#define breakpoint asm volatile("brk #0")

void dbg_main() //Only to be called if c_irq_handler can't figure out what it is

#endif

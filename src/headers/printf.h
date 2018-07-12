#ifndef PRINTF_H
#define PRINTF_H

#include "project.h"

void printf(char *fmt, ...);
size_t ammount_to_alloc_with_symbols(char *str, __builtin_va_list args);

#endif

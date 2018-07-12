#ifndef SPRINTF_H
#define SPRINTF_H

unsigned int sprintf(char *dst, char* fmt, ...);
unsigned int vsprintf(char *dst,char* fmt, __builtin_va_list args);

#endif

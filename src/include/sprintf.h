#ifndef SPRINTF_H
#define SPRINTF_H

uint32_t sprintf(char *dst, char* fmt, ...);
uint32_t vsprintf(char *dst,char* fmt, __builtin_va_list args);

#endif

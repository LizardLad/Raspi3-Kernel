#ifndef STRING_H
#define STRING_H

void itoa(int32_t n, char s[]);
int strlen(char *s);
void reverse(char s[]);
char *strtok_r(char *input, char *delim, char **save_ptr);

#endif

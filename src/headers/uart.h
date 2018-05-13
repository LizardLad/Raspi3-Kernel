#ifndef UART_H
#define UART_H

void uart_init();
void uart_send(unsigned int c);
char uart_getc();
void uart_puts(char *s);
void uart_hex(unsigned int d);
void uart_dump(void *ptr);

#endif

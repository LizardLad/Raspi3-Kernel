#include "project.h"
//================================================
//Delay Functions
//================================================
#define (kernel_wait_cylces(number_of_cycles_to_wait)) 	(wait_cycles(number_of_cycles_to_wait)
#define (kernel_wait_usec(usec)) 				(wait_usec(usec))
#define (kernel_wait_usec_system_timer(num_micros))	(wait_usec_system_timer(num_micros))
//================================================
//Get count of microseconds since board start
//Arduino compatible
//================================================
#define (kernel_micros()) 				(micros())
//================================================
//FAT32 and FAT16 functions to access SD card
//================================================
#define (kernel_fat_getpartition()) 			(fat_getpartition())
#define (kernel_fat_getcluster(fn))			(fat_getcluster(fn))
#define (kernel_fat_listdirectory())			(fat_listdirectory())
#define (kernel_fat_readfile(cluster))			(fat_readfile(cluster))
//================================================
//GPIO functions
//================================================
#define (kernel_gpio_setup(gpio_pin, mode))		(gpio_setup(gpio_setup, mode))
#define (kernel_gpio_input(gpio_pin))			(gpio_input(gpio_pin))
#define (kernel_gpio_output(gpio_pin, on))		(gpio_ouput(gpio_pin, on))
//TODO get gpio states of output and mode
//================================================
//ACT LED functions
//================================================
#define (kernel_set_ACT_LED(on))			(set_ACT_LED(on))
//================================================
//Framebuffer functions
//================================================
#define (kernel_lfb_init())				(lfb_init())
#define (kernel_lfb_draw_pixel(x, y, r, g, b))		(lfb_draw_pixel(x, y, r, g, b))
#define (kenel_lfb_print(x, y, s))			(lfb_print(x, y, s)) //WARNING this is highly discouraged use printf instead
#define (kernel_lfb_hex(x, y, d))			(lfb_hex(x, y, d)) //Please use printf instead
#define (kernel_lfb_dump(ptr))				(lfb_dump(ptr))
//================================================
//Memory allocation
//================================================
#define (kernel_memory_alloc_init())			(memory_alloc_init())
//There are no other special defines here as this complies to the ISO standard for libc
//malloc
//free
//================================================
//Print funcitons
//================================================
//No defines here the functions are as follows
//printf
//================================================
//String formatting 
//================================================
//vsprintf is here
//so is sprintf
//strlen
//reverse
//itoa
//strtok_r
//================================================
//System functions
//================================================
#define (kernel_set_cpu_clock(clock_rate))		(set_cpu_clock(clock_rate))
#define (kernel_get_cpu_max_clock())			(get_cpu_max_clock())
#define (kernel_clocks_init())				(clocks_init())
//================================================
//UART functions
//================================================
#define (kernel_uart_init())				(uart_init())
#define (kernel_uart_send(c))				(uart_send(c))
#define (kernel_uart_getc())				(uart_getc())
#define (kernel_uart_hex(d))				(uart_hex(d))
#define (kernel_uart_dump(ptr))				(uart_dump(ptr))
//================================================
//Kernel GPU mailbox functions will not be exposed!
//If a function requires a mailbox it shall be implemented
//in the kernel itself so incorrect mailbox calls will
//not be made!
//================================================

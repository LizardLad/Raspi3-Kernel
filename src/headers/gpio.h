#ifndef GPIO_H
#define GPIO_H

#include "stdbool.h"

#define MMIO_BASE       0x3F000000

#define GPFSEL0         ((volatile unsigned int*)(MMIO_BASE+0x00200000))
#define GPFSEL1         ((volatile unsigned int*)(MMIO_BASE+0x00200004))
#define GPFSEL2         ((volatile unsigned int*)(MMIO_BASE+0x00200008))
#define GPFSEL3         ((volatile unsigned int*)(MMIO_BASE+0x0020000C))
#define GPFSEL4         ((volatile unsigned int*)(MMIO_BASE+0x00200010))
#define GPFSEL5         ((volatile unsigned int*)(MMIO_BASE+0x00200014))
#define GPSET0          ((volatile unsigned int*)(MMIO_BASE+0x0020001C))
#define GPSET1          ((volatile unsigned int*)(MMIO_BASE+0x00200020))
#define GPCLR0          ((volatile unsigned int*)(MMIO_BASE+0x00200028))
#define GPLEV0          ((volatile unsigned int*)(MMIO_BASE+0x00200034))
#define GPLEV1          ((volatile unsigned int*)(MMIO_BASE+0x00200038))
#define GPEDS0          ((volatile unsigned int*)(MMIO_BASE+0x00200040))
#define GPEDS1          ((volatile unsigned int*)(MMIO_BASE+0x00200044))
#define GPHEN0          ((volatile unsigned int*)(MMIO_BASE+0x00200064))
#define GPHEN1          ((volatile unsigned int*)(MMIO_BASE+0x00200068))
#define GPPUD           ((volatile unsigned int*)(MMIO_BASE+0x00200094))
#define GPPUDCLK0       ((volatile unsigned int*)(MMIO_BASE+0x00200098))
#define GPPUDCLK1       ((volatile unsigned int*)(MMIO_BASE+0x0020009C))

#define HIGH			1
#define LOW			0

typedef enum {
	GPIO_INPUT = 0b000,								// 0
	GPIO_OUTPUT = 0b001,								// 1
	GPIO_ALTFUNC5 = 0b010,								// 2
	GPIO_ALTFUNC4 = 0b011,								// 3
	GPIO_ALTFUNC0 = 0b100,								// 4
	GPIO_ALTFUNC1 = 0b101,								// 5
	GPIO_ALTFUNC2 = 0b110,								// 6
	GPIO_ALTFUNC3 = 0b111								// 7
} GPIOMODE;

bool gpio_setup(unsigned int gpio, GPIOMODE mode);
bool gpio_input(unsigned int gpio);
bool gpio_output(unsigned int gpio, bool on);

void pinMode(unsigned int gpio_pin, GPIOMODE mode);
void digitalWrite(unsigned int gpio_pin, bool value);
bool digitalRead(unsigned int gpio_pin);

#endif

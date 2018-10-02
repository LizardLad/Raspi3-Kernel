#ifndef GPIO_H
#define GPIO_H

#include "stdbool.h"
#include "stdint.h"

#define MMIO_BASE       0x3F000000

#define GPFSEL0         ((volatile uint32_t*)(MMIO_BASE+0x00200000))
#define GPFSEL1         ((volatile uint32_t*)(MMIO_BASE+0x00200004))
#define GPFSEL2         ((volatile uint32_t*)(MMIO_BASE+0x00200008))
#define GPFSEL3         ((volatile uint32_t*)(MMIO_BASE+0x0020000C))
#define GPFSEL4         ((volatile uint32_t*)(MMIO_BASE+0x00200010))
#define GPFSEL5         ((volatile uint32_t*)(MMIO_BASE+0x00200014))
#define GPSET0          ((volatile uint32_t*)(MMIO_BASE+0x0020001C))
#define GPSET1          ((volatile uint32_t*)(MMIO_BASE+0x00200020))
#define GPCLR0          ((volatile uint32_t*)(MMIO_BASE+0x00200028))
#define GPLEV0          ((volatile uint32_t*)(MMIO_BASE+0x00200034))
#define GPLEV1          ((volatile uint32_t*)(MMIO_BASE+0x00200038))
#define GPEDS0          ((volatile uint32_t*)(MMIO_BASE+0x00200040))
#define GPEDS1          ((volatile uint32_t*)(MMIO_BASE+0x00200044))
#define GPHEN0          ((volatile uint32_t*)(MMIO_BASE+0x00200064))
#define GPHEN1          ((volatile uint32_t*)(MMIO_BASE+0x00200068))
#define GPPUD           ((volatile uint32_t*)(MMIO_BASE+0x00200094))
#define GPPUDCLK0       ((volatile uint32_t*)(MMIO_BASE+0x00200098))
#define GPPUDCLK1       ((volatile uint32_t*)(MMIO_BASE+0x0020009C))

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

typedef struct
{
	uint8_t gpio_pin_1 : 1;
	uint8_t gpio_pin_2 : 1;
	uint8_t gpio_pin_3 : 1;
	uint8_t gpio_pin_4 : 1;
	uint8_t gpio_pin_5 : 1;
	uint8_t gpio_pin_6 : 1;
	uint8_t gpio_pin_7 : 1;
	uint8_t gpio_pin_8 : 1;
	uint8_t gpio_pin_9 : 1;
	uint8_t gpio_pin_10 : 1;
	uint8_t gpio_pin_11 : 1;
	uint8_t gpio_pin_12 : 1;
	uint8_t gpio_pin_13 : 1;
	uint8_t gpio_pin_14 : 1;
	uint8_t gpio_pin_15 : 1;
	uint8_t gpio_pin_16 : 1;
	uint8_t gpio_pin_17 : 1;
	uint8_t gpio_pin_18 : 1;
	uint8_t gpio_pin_19 : 1;
	uint8_t gpio_pin_20 : 1;
	uint8_t gpio_pin_21 : 1;
	uint8_t gpio_pin_22 : 1;
	uint8_t gpio_pin_23 : 1;
	uint8_t gpio_pin_24 : 1;
	uint8_t gpio_pin_25 : 1;
	uint8_t gpio_pin_26 : 1;
	uint8_t gpio_pin_27 : 1;
	uint8_t gpio_pin_28 : 1;
	uint8_t gpio_pin_29 : 1;
	uint8_t gpio_pin_30 : 1;
	uint8_t gpio_pin_31 : 1;
	uint8_t gpio_pin_32 : 1;
	uint8_t gpio_pin_33 : 1;
	uint8_t gpio_pin_34 : 1;
	uint8_t gpio_pin_35 : 1;
	uint8_t gpio_pin_36 : 1;
	uint8_t gpio_pin_37 : 1;
	uint8_t gpio_pin_38 : 1;
	uint8_t gpio_pin_39 : 1;
	uint8_t gpio_pin_40 : 1;
	uint8_t gpio_pin_41 : 1;
	uint8_t gpio_pin_42 : 1;
	uint8_t gpio_pin_43 : 1;
	uint8_t gpio_pin_44 : 1;
	uint8_t gpio_pin_45 : 1;
	uint8_t gpio_pin_46 : 1;
	uint8_t gpio_pin_47 : 1;
	uint8_t gpio_pin_48 : 1;
	uint8_t gpio_pin_49 : 1;
	uint8_t gpio_pin_50 : 1;
	uint8_t gpio_pin_51 : 1;
	uint8_t gpio_pin_52 : 1;
	uint8_t gpio_pin_53 : 1;
	uint8_t gpio_pin_54 : 1;
} get_gpio_pin_status_t;

get_gpio_pin_status_t gpio_pin_status;

bool gpio_setup(uint32_t gpio_pin, GPIOMODE mode);
bool gpio_input(uint32_t gpio);
bool gpio_output(uint32_t gpio, bool on);

uint8_t get_gpio_pin_status(uint32_t gpio_pin);

void pinMode(uint32_t gpio_pin, GPIOMODE mode);
void digitalWrite(uint32_t gpio_pin, bool value);
bool digitalRead(uint32_t gpio_pin);

#endif

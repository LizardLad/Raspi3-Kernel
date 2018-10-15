#include "include/stdint.h"
#include "include/stdbool.h"
#include "include/mbox.h"
#include "include/uart.h"
#include "include/gpio.h"
#include "include/delays.h"

/* PL011 UART registers */
#define UART0_DR        ((volatile uint32_t*)(MMIO_BASE+0x00201000))
#define UART0_FR        ((volatile uint32_t*)(MMIO_BASE+0x00201018))
#define UART0_IBRD      ((volatile uint32_t*)(MMIO_BASE+0x00201024))
#define UART0_FBRD      ((volatile uint32_t*)(MMIO_BASE+0x00201028))
#define UART0_LCRH      ((volatile uint32_t*)(MMIO_BASE+0x0020102C))
#define UART0_CR        ((volatile uint32_t*)(MMIO_BASE+0x00201030))
#define UART0_IMSC      ((volatile uint32_t*)(MMIO_BASE+0x00201038))
#define UART0_ICR       ((volatile uint32_t*)(MMIO_BASE+0x00201044))

/**
 * Set baud rate and characteristics (115200 8N1) and map to GPIO
 */
void uart_init()
{
	register uint32_t r;
	
	/* initialize UART */
	*UART0_CR = 0;         // turn off UART0

	/* set up clock for consistent divisor values */
	mailbox[0] = 8*4;
	mailbox[1] = MBOX_REQUEST;
	mailbox[2] = MBOX_TAG_SET_CLKRATE; // set clock rate
	mailbox[3] = 12;
	mailbox[4] = 8;
	mailbox[5] = CLK_UART_ID;	// UART clock
	mailbox[6] = 4000000;		// 4Mhz
	mailbox[7] = MBOX_TAG_LAST;
	mailbox_call(MBOX_CH_PROP);

	uint32_t buffer[5] = { 0 };
	if(!mailbox_tag_message(buffer, 5, MBOX_TAG_SET_CLKRATE, 8, 8, CLK_UART_ID, 4000000))
	{
		return; //Fails silently be aware!
	}

	/* map UART0 to GPIO pins */
	r=*GPFSEL1;
	r&=~((7<<12)|(7<<15)); // gpio14, gpio15
	r|=(4<<12)|(4<<15);    // alt0
	*GPFSEL1 = r;
	*GPPUD = 0;            // enable pins 14 and 15
	wait_cycles(150);
	*GPPUDCLK0 = (1<<14)|(1<<15);
	wait_cycles(150);
	*GPPUDCLK0 = 0;        // flush GPIO setup

	*UART0_ICR = 0x7FF;    // clear interrupts
	*UART0_IBRD = 2;       // 115200 baud
	*UART0_FBRD = 0xB;
	*UART0_LCRH = 0b11<<5; // 8n1
	*UART0_CR = 0x301;     // enable Tx, Rx, FIFO
}

/**
 * Send a character
 */
void uart_send(uint32_t c) {
	/* wait until we can send */
	do{asm volatile("nop");}while(*UART0_FR&0x20);
	/* write the character to the buffer */
	*UART0_DR=c;
}

/**
 * Receive a character
 */
char uart_getc() {
	char r;
	/* wait until something is in the buffer */
	do{asm volatile("nop");}while(*UART0_FR&0x10);
	/* read it and return */
	r=(char)(*UART0_DR);
	/* convert carrige return to newline */
	return r=='\r'?'\n':r;
}

/**
 * Display a string
 */
void uart_puts(char *s) {
	//TODO semaphore here
	while(*s) {
		/* convert newline to carrige return + newline */
		if(*s=='\n')
			uart_send('\r');
		uart_send(*s++);
	}
}

/**
 * Display a binary value in hexadecimal
 */
void uart_hex(uint32_t d) {
	uint32_t n;
	int32_t c;
	for(c=28;c>=0;c-=4) {
		// get highest tetrad
		n=(d>>c)&0xF;
		// 0-9 => '0'-'9', 10-15 => 'A'-'F'
		n+=n>9?0x37:0x30;
		uart_send(n);
	}
}

/**
 * Dump memory
 */
void uart_dump(void *ptr)
{
	uint64_t a,b,d;
	unsigned char c;
	for(a=(uint64_t)ptr;a<(uint64_t)ptr+512;a+=16) {
		uart_hex(a); uart_puts(": ");
		for(b=0;b<16;b++) {
			c=*((unsigned char*)(a+b));
			d=(uint32_t)c;d>>=4;d&=0xF;d+=d>9?0x37:0x30;uart_send(d);
			d=(uint32_t)c;d&=0xF;d+=d>9?0x37:0x30;uart_send(d);
			uart_send(' ');
			if(b%4==3)
				uart_send(' ');
		}
		for(b=0;b<16;b++) {
			c=*((unsigned char*)(a+b));
			uart_send(c<32||c>=127?'.':c);
		}
		uart_send('\r');
		uart_send('\n');
	}
}

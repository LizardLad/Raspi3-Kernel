#include <stdint.h>
#include "include/sound.h"
#include "include/gpio.h"

#define PWM_BASE		0x20C000	// PWM Base Address
#define GPIO_BASE		0x200000	// GPIO Base address
#define CM_BASE			0x101000	// Clock manager base address

#define PWM_CTL			0		// PWM Control ofsfet 0
#define PWM_STA			1		// PWM Status offset +04 which is 1 on a uint32_t array
#define PWM_RNG1		4		// PWM Channel 1 Range +0x10 which is 4 on a uint32_t array
#define PWM_FIF1		6		// PWM FIFO Input +18 which is 6 on a uint32_t array
#define PWM_RNG2		8		// PWM Channel 2 Range +0x20 which is 8 on a uint32_t array

#define PWM_FULL1		0x1		// PWM full status flag

#define PWM_PWEN1		0x1		// PWM Control: Channel 1 Enable
#define PWM_USEF1		0x20		// PWM Control: Channel 1 Use Fifo
#define PWM_CLRF1		0x40		// PWM Control: Clear Fifo
#define PWM_PWEN2		0x100		// PWM Control: Channel 2 Enable
#define PWM_USEF2		0x2000		// PWM Control: Channel 2 Use Fifo

#define GPIO_GPFSEL4		4		// GPIO Function Select 4 +0x10 which is 1 in a uint32_t array
#define GPIO_FSEL0_ALT0 	0x4		// GPIO Function Select: GPIO Pin X0 Takes Alternate Function 0
#define GPIO_FSEL5_ALT0 	0x20000		// GPIO Function Select: GPIO Pin X5 Takes Alternate Function 0

#define CM_PASSWORD		0x5A000000	// Clock Control: Password "5A"
#define CM_ENAB			0x10		// Clock Control: Enable The Clock Generator
#define CM_SRC_OSCILLATOR	0x01		// Clock Control: Clock Source = Oscillator
#define CM_SRC_PLLCPER		0x05		// Clock Control: Clock Source = PLLC Per

#define CM_PWMCTL		0x28		// Clock Manager PWM Clock Control offset +0x0A0 which is 0x28 in a uint32_t array
#define CM_PWMDIV		0x29		// Clock Manager PWM Clock Divisor offset +0x0A4 which is 0x29 in a uint32_t array

volatile uint32_t* PWM	= (uint32_t*)(MMIO_BASE + PWM_BASE);
volatile uint32_t* GPIO	= (uint32_t*)(MMIO_BASE + GPIO_BASE);
volatile uint32_t* CM	= (uint32_t*)(MMIO_BASE + CM_BASE);

const uint16_t *audio_start;
const uint16_t *audio_end;

void init_audio_jack(void)
{
	GPIO[GPIO_GPFSEL4] = (GPIO_FSEL0_ALT0 | GPIO_FSEL5_ALT0); // Set alt function to pins
	CM[CM_PWMDIV] = (CM_PASSWORD | 0x2000);    // Set Clock
	CM[CM_PWMCTL] = (CM_PASSWORD | CM_ENAB | (CM_SRC_OSCILLATOR + CM_SRC_PLLCPER)); // Use 650MHz PLLC Clock
	PWM[PWM_RNG1] = 0x1624; // Range = 12bit 44100Hz Stereo
	PWM[PWM_RNG2] = 0x1624; // Range = 12bit 44100Hz Stereo
	PWM[PWM_CTL] = (PWM_USEF2 | PWM_PWEN2 | PWM_USEF1 | PWM_PWEN1 | PWM_CLRF1);
}

void play_audio(void *x)
{
	while (1) 
	{
		uint32_t i = 0;
		while (i < (audio_end - audio_start))			// The subtraction is ptr arithmetic as its uint16_t* its half what it looks like
		{
			PWM[PWM_FIF1] = audio_start[i] >> 4;		// Write 16 bit sample right shift 4 for 12 bit out
			i++;
			PWM[PWM_FIF1] = audio_start[i] >> 4;		// Write 16 bit sample right shift 4 for 12 bit out
			i++;
			while ((PWM[PWM_STA] & PWM_FULL1) == 1) {};	// While fifo full wait
		}
	}
}

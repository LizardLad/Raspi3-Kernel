#include "headers/project.h"

int init_audio_jack()
{
	*((volatile unsigned int *)(MMIO_BASE + 0x200000 + 0x10)) = (0x4 | 0x200000);

	*CM_PWMDIV = CM_PASS | 0x2000; //Set clock block 0

	*CM_PWMCTL = (CM_PASS | 0x10) | (0x01 + 0x05); //Set clock block 1

	*(PWM_BASE + 0x10) = 0x2C48;
	*(PWM_BASE + 0x20) = 0x2C48;

	*(PWM_BASE + 0x0) = 0x20 + 0x100 + 0x2000 + 0x1 + 0x40;

	lfb_print(0, 2, "Well there were no unaligned exeptions");

	return 0;
}

//I really hope this works!
int play_16bit_unsigned_audio(char *start, char *end)
{
	if(end < start) return 1;
	lfb_print(0, 3, "End isn't less than start.");
	if((start - end) % 2 != 0) return 2;
	lfb_print(0, 4, "Is a multiple of two so it is 16bit");
	//FIFO write
	for(int i = 0; &(start[i]) != end; i++)
	{
		uint8_t sample_low = start[i];
		uint8_t sample_high = start[i++];

		uint16_t sample = sample_low | (sample_high << 8);

		sample >>= 2;
		*PWM_FIF1 = sample;
		//FIFO wait
		while(*PWM_STA != 0x1);
	}
	lfb_print(0, 5, "Completed Audio");
	return 0;
}

#include "headers/project.h"

int init_audio_jack()
{
	*((volatile uint32_t *)(MMIO_BASE + 0x200000 + 0x10)) = (0x4 | 0x200000);

	*CM_PWMDIV = CM_PASS | 0x2000; //Set clock block 0

	*CM_PWMCTL = (CM_PASS | 0x10) | (0x01 + 0x05); //Set clock block 1

	*(PWM_BASE + 0x10) = 0x2C48;
	*(PWM_BASE + 0x20) = 0x2C48;

	*(PWM_BASE + 0x0) = 0x20 + 0x100 + 0x2000 + 0x1 + 0x40;

	printf("[INFO] Audio Init Finished");

	return 0;
}

//I really hope this works!
int32_t play_16bit_unsigned_audio(char *start, char *end)
{
	if(end < start) 
	{
		printf("[ERROR] End is less than start.");
		return 1;
	}
	if((start - end) % 2 == 0)
	{
		printf("[ERROR] Isn't a multiple of two so it isn't 16bit");
		return 2;
	}
	//FIFO write
	for(int32_t i = 0; &(start[i]) != end; i++)
	{
		uint8_t sample_low = start[i];
		uint8_t sample_high = start[i++];

		uint16_t sample = sample_low | (sample_high << 8);

		sample >>= 2;
		*PWM_FIF1 = sample;
		//FIFO wait
		while(*PWM_STA != 0x1);
	}
	printf("[INFO] Completed Audio");
	return 0;
}

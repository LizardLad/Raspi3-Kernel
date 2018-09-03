#include "headers/project.h"

void init_audio_jack_c()//ERROR IN HERE
{
	//Set phone jack to pwm output
	uint32_t *gpio_addr = (uint32_t *)(PERIPHERAL_BASE + GPIO_BASE);
       	uint32_t *gpio_gpfsel4_addr = gpio_addr + GPIO_GPFSEL4;
	*gpio_gpfsel4_addr = GPIO_FSEL0_ALT0 | GPIO_FSEL5_ALT0;

	//Set clock
	uint32_t *clock_manager_addr = (uint32_t *)(((PERIPHERAL_BASE + CM_BASE) & 0x0000FFFF) | ((PERIPHERAL_BASE + CM_BASE) & 0xFFFF0000));
	*(clock_manager_addr + CM_PWMDIV) = (CM_PASSWORD | 0x2000);

	*(clock_manager_addr + CM_PWMCTL) = ((CM_PASSWORD | CM_ENAB) | (CM_SRC_OSCILLATOR + CM_SRC_PLLCPER));

	//Set PWM
	uint32_t *pwm_manager_addr = (uint32_t *)(((PERIPHERAL_BASE + PWM_BASE) & 0x0000FFFF) | ((PERIPHERAL_BASE + PWM_BASE) & 0xFFFF0000));
	*(pwm_manager_addr + PWM_RNG1) = 0x1624;
	*(pwm_manager_addr + PWM_RNG2) = 0x1624;

	*(pwm_manager_addr + PWM_CTL) = PWM_USEF2 + PWM_PWEN2 + PWM_USEF1 + PWM_PWEN1 + PWM_CLRF1;

	printf("[INFO] Audio Init Finished");
}


int32_t play_16bit_unsigned_audio(uint16_t *start, uint16_t *end)
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

	uint16_t *end_of_file = (uint16_t *)(uint64_t)(((uint32_t)(uintptr_t)end & 0x0000FFFF) | ((uint32_t)(uintptr_t)end & 0xFFFF0000));

	//FIFO write
	while(start != end_of_file)
	{
		uint16_t sample = start[0];
		sample >>= 3;
		*(uint32_t *)((((uint32_t)(PERIPHERAL_BASE + PWM_BASE) & 0x0000FFFF) | ((uint32_t)(PERIPHERAL_BASE + PWM_BASE) & 0xFFFF0000)) + PWM_FIF1) = sample;
		
		start++;
		sample = start[0];
		sample >>= 3;
		*(uint32_t *)((((uint32_t)(PERIPHERAL_BASE + PWM_BASE) & 0x0000FFFF) | ((uint32_t)(PERIPHERAL_BASE + PWM_BASE) & 0xFFFF0000)) + PWM_FIF1) = sample;
		
		//FIFO wait
		while(*(uint32_t *)((((uint32_t)(PERIPHERAL_BASE + PWM_BASE) & 0x0000FFFF) | ((uint32_t)(PERIPHERAL_BASE + PWM_BASE) & 0xFFFF0000)) + PWM_STA) != PWM_FULL1);
		start++;
	}
	printf("[INFO] Completed Audio");
	return 0;
}

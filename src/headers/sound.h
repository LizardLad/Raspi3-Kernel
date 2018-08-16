#ifndef SOUND_H
#define SOUND_H

#include "gpio.h"

#define CM_BASE		((volatile uint32_t *)(MMIO_BASE+0x101000))
#define CM_PWMDIV	((volatile uint32_t *)(CM_BASE+0x0A4))
#define CM_PASS		0x5A000000
#define CM_PWMCTL	((volatile uint32_t *)(CM_BASE+0x0A0))
#define PWM_BASE	((volatile uint32_t *)(MMIO_BASE+0x20C000))
#define PWM_RNG1	((volatile uint32_t *)(PWM_BASE+0x10))
#define PWM_RNG2	((volatile uint32_t *)(PWM_BASE+0x20))
#define PWM_FIF1	((volatile uint32_t *)(PWM_BASE+0x18))
#define PWM_STA		((volatile uint32_t *)(PWM_BASE+0x4))

extern volatile unsigned char _binary_src_audio_The_Amazons_bin_start, _binary_src_audio_The_Amazons_bin_end;

int init_audio_jack();
int play_16bit_unsigned_audio(char *start, char *end);

#endif

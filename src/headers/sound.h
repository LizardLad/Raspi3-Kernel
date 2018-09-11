#ifndef SOUND_H
#define SOUND_H

#include "gpio.h"

#define PERIPHERAL_BASE		((volatile uint32_t)(0x3F000000))
#define GPIO_BASE		((volatile uint32_t)(0x200000))
#define GPIO_FSEL0_ALT0		((volatile uint32_t)(0x4))
#define GPIO_FSEL5_ALT0		((volatile uint32_t)(0x20000))
#define GPIO_GPFSEL4		((volatile uint32_t)(0x10))

#define CM_BASE			((volatile uint32_t)(0x101000))
#define CM_PASSWORD		((volatile uint32_t)(0x5A000000))
#define CM_PWMDIV		((volatile uint32_t)(0x0A4))
#define CM_ENAB			((volatile uint32_t)(0x10))
#define CM_SRC_OSCILLATOR	((volatile uint32_t)(0x01))
#define CM_SRC_PLLCPER		((volatile uint32_t)(0x05))
#define CM_PWMCTL		((volatile uint32_t)(0x0A0))

#define PWM_BASE		((volatile uint32_t)(0x20C000))
#define PWM_RNG1		((volatile uint32_t)(0x10))
#define PWM_RNG2		((volatile uint32_t)(0x20))
#define PWM_USEF2		((volatile uint32_t)(0x2000))
#define PWM_PWEN2		((volatile uint32_t)(0x100))
#define PWM_USEF1		((volatile uint32_t)(0x20))
#define PWM_PWEN1		((volatile uint32_t)(0x1))
#define PWM_CLRF1		((volatile uint32_t)(0x40))
#define PWM_CTL			((volatile uint32_t)(0x0))

#define PWM_FIF1		((volatile uint32_t)(0x18))
#define PWM_STA			((volatile uint32_t)(0x4))
#define PWM_FULL1		((volatile uint32_t)(0x1))

extern __attribute__((aligned(2))) uint16_t _binary_src_audio_The_Amazons_bin_start, _binary_src_audio_The_Amazons_bin_end;
extern __attribute__((aligned(2))) uint16_t _binary_src_audio_Interlude_bin_start, _binary_src_audio_Interlude_bin_end;

void init_audio_jack_c();
void init_audio_jack();
int play_16bit_unsigned_audio(uint16_t *start, uint16_t *end);
void play_audio();

#endif

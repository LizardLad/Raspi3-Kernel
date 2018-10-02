#ifndef _SOUND_H_
#define _SOUND_H_

#ifdef __cplusplus								// If we are including to a C++
extern "C" {									// Put extern C directive wrapper around
#endif

#include "stdint.h"

#if !defined(__STRINGIFY2)
#define __STRINGIFY2(__p) #__p
#define __STRINGIFY(__p) __STRINGIFY2(__p)
#endif
	
#define INCLUDE_BINARY_FILE(__variable, __fileName, __section)					 \
__asm__ (                                                                        \
    ".pushsection " __section                                          "\n\t"    \
    ".globl " __STRINGIFY(__variable) "_start;"                        "\n\t"    \
    ".balign 4"                                                        "\n\t"    \
    __STRINGIFY(__variable) "_start: .incbin \"" __fileName "\""       "\n\t"    \
    ".globl " __STRINGIFY(__variable) "_end;"		                   "\n\t"    \
    __STRINGIFY(__variable) "_end: .4byte 0;"                          "\n\t"    \
    ".balign 4"                                                        "\n\t"    \
    ".popsection"                                                      "\n\t"    \
);\
extern const uint16_t __variable ## _start;\
extern const uint16_t __variable ## _end;                             

extern const uint16_t *audio_start;
extern const uint16_t *audio_end;

void play_audio (void);
void init_audio_jack(void);


#ifdef __cplusplus								// If we are including to a C++ file
}												// Close the extern C directive wrapper
#endif

#endif

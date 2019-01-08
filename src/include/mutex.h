#ifndef MUTEX_H
#define MUTEX_H

#include <stdint.h>

typedef uint8_t tid_t;

typedef uint32_t sem_t;

typedef struct mutex_r_tt {
	sem_t sem;
	volatile tid_t owner;
	uint32_t count;
} mutex_r_t;

#endif

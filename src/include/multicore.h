#ifndef MULTICORE_H
#define MULTICORE_H

#include <stdint.h>

union caller_func {					// We have a number of different functions
	void* val;						// Used to set
	void (*func0) (void);			// Type 0 function is void (*) (void)
	void (*func1) (void*);			// Type 1 function is void (*) (void*)
	void (*func2) (void*, void*);	// Type 2 function is void (*) (void*, void*)
};

/* Inter-core message struct */
struct icm_message {
	union caller_func func;			// Function to call
	uint32_t functype;				// Function type above 0,1,2
	uint32_t response;				// response set to 1 when core accepts message
	void* param1;					// param1 for type 1 & 2 func
	void* param2;					// param2 for type 2 func
};

void multicore_init();
int get_core_id();

int core_execute(char core_id, uint32_t functype, void *func, void *param_1, void *param_2);

#endif

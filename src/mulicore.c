#include "include/stdbool.h"
#include "include/multicore.h"

bool core_state[4] = { 0 };

volatile bool *core0_ready = &core_state[0];
volatile bool *core1_ready = &core_state[1];
volatile bool *core2_ready = &core_state[2];
volatile bool *core3_ready = &core_state[3];

void start_other_3_cores()
{
	asm(	"mov	x1, #0xe0\n"\
		"mov    x2, #0x80000\n"\
		"str    x2, [x1]\n"\
		"sev\n");
	
	while (*core1_ready == false) {asm volatile("nop");}
		
	asm(	"mov	x1, #0xe8\n"\
		"mov    x2, #0x80000\n"\
		"str	x2, [x1]\n"\
		"sev\n");
	
	while (*core2_ready == false) {asm volatile("nop");}
	
	asm(	"mov	x1, 0xf0\n"\
		"mov    x2, #0x80000\n"\
		"str	x2, [x1]\n"\
		"sev");
	
	while (*core3_ready == false) {asm volatile("nop");}
}

int get_core_id()
{
	//This function retrieves the core id
	//This is very useful for debugging
	
	int core_id;

	asm volatile(	"mrs x1, mpidr_el1\n"\
			"and %0, x1, #0x3" : "=r"(core_id) : :"x1");

	return core_id;
}

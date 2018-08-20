#include "headers/project.h"

void start_other_3_cores()
{
	asm(	"mov	x1, #0xe0\n"\
		"mov    x2, #0x80000\n"\
		"str    x2, [x1]\n"\
		"mov	x1, #0xe8\n"\
		"str	x2, [x1]\n"\
		"mov	x1, 0xf0\n"\
		"str	x2, [x1]\n"\
		"sev");
}

/*int core_execute(uint8_t core_num, void (*func)())
*{
*	if(core == 1)
*	{
*		void *a = 0xe0;	//Load core 1 hibernation address into char *a
*		*a = func;	//Load the function address into the hibernation address
*		asm("sev");	//Wake all cores. Cores with an address will start execution there
*		return 1; //Return 0x1 for success on core 1
*	}
*	else if(core == 2)
*	{
*		void *b = 0xe8;	//Load core 2 hibernation address into char *b	
*		*b = func;	//Load the function address into the hibernation address
*		asm("sev");	//Wake all cores. Cores with an address will start execution there
*		return 2; //Return 0x2 for success on core 2
*	}
*	else if(core == 3)
*	{
*		void *c = 0xf0; //Load core 3 hibernation address into char *c
*		*c = func;	//Load the address of the target function into *c
*		sam("sev");	//Wake all cores. Cores with an address in their hibernation address will jump to that address
*		return 3; //Return 0x3 in the event that core 3 jumped to that address and is executing
*	}
*	return 0; //Return NULL in the event that a core other than 1, 2, or 3 was called to execute code
*}
*/

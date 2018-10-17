#include <stdbool.h>
#include "include/multicore.h"
#include "include/printf.h"
#include "include/delays.h"
#include "include/start.h"
#include "include/mmu.h"
#include "include/lfb.h"

static uint32_t core_execute_lock[3] = { 0 };
volatile bool core_execute_state[3] = { 0 };
volatile bool core_ready[3] = { 0 };

void (*core1_jump_func_ptr) (void *);
void (*core2_jump_func_ptr) (void *);
void (*core3_jump_func_ptr) (void *);

void *core1_data;
void *core2_data;
void *core3_data;

void multicore_init()
{
	//Executing on core 0
	
	asm volatile (	"mov	x1, #0xe0\n"\
			"mov    x2, #0x80000\n"\
			"str    x2, [x1]\n");
	asm volatile (	"dc civac, %0" : : "r" (0xe0) : "memory");		
	asm volatile (	"sev\n");
	
	while (core_ready[0] == false) {asm volatile("nop");}

	printf("[CORE %d] [INFO] CORE 1 acknoledged start\n", get_core_id());
		
	asm volatile (	"mov	x1, #0xe8\n"\
			"mov    x2, #0x80000\n"\
			"str	x2, [x1]\n");
	asm volatile (	"dc civac, %0" : : "r" (0xe8) : "memory");		
	asm volatile (	"sev\n");
	
	while (core_ready[1] == false) {asm volatile("nop");}
	printf("[CORE %d] [INFO] CORE 2 acknoledged start\n", get_core_id());
	
	asm volatile (	"mov	x1, 0xf0\n"\
			"mov    x2, #0x80000\n"\
			"str	x2, [x1]\n");
	asm volatile (	"dc civac, %0" : : "r" (0xf0) : "memory");		
	asm volatile (	"sev\n");
	
	while (core_ready[2] == false) {asm volatile("nop");}
	printf("[CORE %d] [INFO] CORE 3 acknoledged start\n", get_core_id());
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

void multicore_mmu_init()
{
	/* cores enter here .. turn on mmu and then park */
	volatile uint32_t *mailbox = (uint32_t *)0x400000CC;
	void(*func) (void) = 0;
	mmu_init();
	do
	{
		asm("wfe");
		func = (void*)(uintptr_t)mailbox[get_core_id() * 4]; 
		mailbox[get_core_id() * 4] = 0;
	} while (func ==  0);
	func();
}

void get_core_ready()
{
	switch(get_core_id())
	{
		case 1:
			core_ready[0] = true;
			break;
		case 2:
			core_ready[1] = true;
			break;
		case 3:
			core_ready[2] = true;
			break;
		default:
			break; //I don't know how this would execute
	}
}

//==============================================================================================//
//	Documentation of core_execute(char core_id, void (*func_ptr) (void *), void *data_ptr)	//
//												//
//	core_execute has two paramaters. The first is the number of the core			//
//	to execute on, the second is a pointer to the function to execute.			//
//	The second parameter is the name of the function to call for example			//
//	it would be called in the following manner:						//
//	core_execute(1, printf_core_wrapper, (void *)data)					//
//												//
//	The data is then passed as an argument to the function to be executed.			//
//==============================================================================================//

int core_execute(char core_id, void (*func_ptr) (void *), void *data_ptr) //This will only run on core 0
{
	if(get_core_id()) return 1; //Not running on core 0
	switch(core_id)
	{
		case 1:
			semaphore_inc(&core_execute_lock[0]);
			core1_jump_func_ptr = func_ptr;
			core1_data = data_ptr;
			core_execute_state[0] = true;
			semaphore_dec(&core_execute_lock[0]);
			break;
		case 2:
			semaphore_inc(&core_execute_lock[1]);
			core2_jump_func_ptr = func_ptr;
			core2_data = data_ptr;
			core_execute_state[1] = true;
			semaphore_dec(&core_execute_lock[1]);
			break;
		case 3:
			semaphore_inc(&core_execute_lock[2]);
			core3_jump_func_ptr = func_ptr;
			core3_data = data_ptr;
			core_execute_state[2] = true;
			semaphore_dec(&core_execute_lock[2]);
			break;
		default:
			return 2; //Invalid core id
	}
	return 0;
}
			
void core_wait_for_instruction()
{
	//Here I have to do some init stuff as well to get the cores ready to opperate normally

	printf("[CORE %d] [INFO] Hello from Core %d\n[CORE %d] [INFO] MMU Online\n", get_core_id(), get_core_id(), get_core_id());
	
	//This is a place the cores come when they are done and relaxing
	bool core_execute_local[3] = { false };
	
	get_core_ready(); //Toggle the ready flag and let another core be released
	
	while(1) //The cores never leave they only branch out and then come back
	{
		switch(get_core_id())
		{
			case 1:
				semaphore_inc(&core_execute_lock[0]);
				if(core_execute_state[0] == true)
				{
					core_execute_state[0] = false;
					core_execute_local[0] = true;
				}
				semaphore_dec(&core_execute_lock[0]);
				if(core_execute_local[0] == true)
				{
					core1_jump_func_ptr(core1_data);
					core_execute_local[0] = false;
				}
				break;
			case 2:
				semaphore_inc(&core_execute_lock[1]);
				if(core_execute_state[1] == true)
				{
					core_execute_state[1] = false;
					core_execute_local[1] = true;
				}
				semaphore_dec(&core_execute_lock[1]);
				if(core_execute_local[1] == true)
				{
					core2_jump_func_ptr(core2_data);
					core_execute_local[1] = false;
				}
				break;
			case 3:
				semaphore_inc(&core_execute_lock[2]);
				if(core_execute_state[2] == true)
				{
					core_execute_state[2] = false;
					core_execute_local[2] = true;
				}
				semaphore_dec(&core_execute_lock[2]);
				if(core_execute_local[2] == true)
				{
					core3_jump_func_ptr(core3_data);
					core_execute_local[2] = false;
				}
				break;
			default:
				//Not valid if this ever occured it isn't being run on the target board
				break;
		}
	}
}

void multicore_init_stage_2()
{
	volatile uint32_t *mailbox = (uint32_t *)0x4000009C;
	mailbox[1 * 4] = (uintptr_t)(void *)core_wait_for_instruction;
	mailbox[2 * 4] = (uintptr_t)(void *)core_wait_for_instruction;
	mailbox[3 * 4] = (uintptr_t)(void *)core_wait_for_instruction;
	asm(	"dmb sy\n"\
		"sev");
}

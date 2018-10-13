#include "include/stdbool.h"
#include "include/multicore.h"
#include "include/printf.h"
#include "include/delays.h"
#include "include/start.h"
#include "include/mmu.h"
#include "include/lfb.h"

static uint32_t core1_execute_lock = 0;
static uint32_t core2_execute_lock = 0;
static uint32_t core3_execute_lock = 0;

bool core_state[4] = { 0 };

volatile bool *core0_ready = &core_state[0];
volatile bool *core1_ready = &core_state[1];
volatile bool *core2_ready = &core_state[2];
volatile bool *core3_ready = &core_state[3];

void (*core1_jump_func_ptr) (void *);
void (*core2_jump_func_ptr) (void *);
void (*core3_jump_func_ptr) (void *);

bool core_execute_state[3] = { 0 };

volatile bool *core1_execute = &core_execute_state[0];
volatile bool *core2_execute = &core_execute_state[1];
volatile bool *core3_execute = &core_execute_state[2];

void *core1_data;
void *core2_data;
void *core3_data;

void multicore_init()
{
	asm(	"mov	x1, #0xe0\n"\
		"mov    x2, #0x80000\n"\
		"str    x2, [x1]\n"\
		"sev\n");
	
	while (*core1_ready == false) {asm volatile("dmb sy");}
		
	lfb_print(0, 0, "1");
	
	asm(	"mov	x1, #0xe8\n"\
		"mov    x2, #0x80000\n"\
		"str	x2, [x1]\n"\
		"sev\n");
	
	while (*core2_ready == false) {asm volatile("dmb sy");}
	
	lfb_print(0, 1, "2");
	
	asm(	"mov	x1, 0xf0\n"\
		"mov    x2, #0x80000\n"\
		"str	x2, [x1]\n"\
		"sev");
	
	while (*core3_ready == false) {asm volatile("dmb sy");}

	lfb_print(0, 2, "3");	
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

void get_core_ready()
{
	switch(get_core_id())
	{
		//Cache stuff required here
		case 1:
			asm volatile ("dc civac, %0" : : "r" (core1_ready) : "memory");
			*core1_ready = true;
			break;
		case 2:
			asm volatile ("dc civac, %0" : : "r" (core2_ready) : "memory");
			*core2_ready = true;
			break;
		case 3:
			asm volatile ("dc civac, %0" : : "r" (core3_ready) : "memory");
			*core3_ready = true;
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
	switch(get_core_id())
	{
		case 1:
			semaphore_inc(&core1_execute_lock);
			core1_jump_func_ptr = func_ptr;
			core1_data = data_ptr;
			*core1_execute = true;
			semaphore_dec(&core1_execute_lock);
			break;
		case 2:
			semaphore_inc(&core2_execute_lock);
			core2_jump_func_ptr = func_ptr;
			core2_data = data_ptr;
			*core2_execute = true;
			semaphore_dec(&core2_execute_lock);
			break;
		case 3:
			semaphore_inc(&core3_execute_lock);
			core3_jump_func_ptr = func_ptr;
			core3_data = data_ptr;
			*core3_execute = true;
			semaphore_dec(&core3_execute_lock);
			break;
		default:
			return 2; //Invalid core id
	}
	return 0;
}
			
void core_wait_for_instruction()
{
	//Here I have to do some init stuff as well to get the cores ready to opperate normally
	//This mostly involves the mmu
	//The mmu is required because semaphores are used in this very function!

	mmu_init(); //The page table has already been created by core 0
	printf("[CORE %d] [INFO] Hello from Core %d\n[CORE %d] [INFO] MMU Online\n", get_core_id(), get_core_id(), get_core_id());
	
	//This is a place the cores come when they are done and relaxing
	bool core1_execute_local = false;
	bool core2_execute_local = false;
	bool core3_execute_local = false;
	
	get_core_ready(); //Toggle the ready flag and let another core be released
	
	while(1) //The cores never leave they only branch out and then come back
	{
		switch(get_core_id())
		{
			case 1:
				semaphore_inc(&core1_execute_lock);
				if(*core1_execute == true)
				{
					*core1_execute = false;
					core1_execute_local = true;
				}
				semaphore_dec(&core1_execute_lock);
				if(core1_execute_local == true)
				{
					core1_jump_func_ptr(core1_data);
					core1_execute_local = false;
				}
				break;
			case 2:
				semaphore_inc(&core2_execute_lock);
				if(*core2_execute == true)
				{
					*core2_execute = false;
					core2_execute_local = true;
				}
				semaphore_dec(&core2_execute_lock);
				if(core2_execute_local == true)
				{
					core2_jump_func_ptr(core2_data);
					core2_execute_local = false;
				}
				break;
			case 3:
				semaphore_inc(&core3_execute_lock);
				if(*core3_execute == true)
				{
					*core3_execute = false;
					core3_execute_local = true;
				}
				semaphore_dec(&core3_execute_lock);
				if(core3_execute_local == true)
				{
					core3_jump_func_ptr(core3_data);
					core3_execute_local = false;
				}
				break;
			default:
				//Not valid if this ever occured it isn't being run on the target board
				break;
		}
	}
}

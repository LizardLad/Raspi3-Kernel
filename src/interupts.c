#include <stdbool.h>
#include "include/printf.h"
#include "include/interupts.h"
#include "include/multicore.h"
#include "include/led.h"


//I'm not yet sure how this works or what I'm doing all of this is experimental

void enable_irq()
{
	asm volatile("msr daifclr, #2");
}

void disable_irq()
{
	asm volatile("msr daifset, #2");
}

void c_irq_handler()
{
	//1: Figure out what did it
	//2: Call a more specific function based on who initiated the call
	//3: If who ever called it isn't in here send to dbg_main()
	//4: Re enable interupts
	//5: return
	printf("[CORE %d] [INFO] Interupt occured!\n", get_core_id());
	if(QA7->TimerControlStatus.IntPending == 1) //Local timer caused the interupt
	{
		timer_interupt_handler();
	}
	else
	{
		dbg_main();
	}
}

bool lit = 0;

void timer_interupt_handler()
{
	//Just flip flopping an LED
	lit = lit ? true : false;
	set_ACT_LED(lit);

	printf("[CORE %d] [INFO] Timer interupt occured!\n", get_core_id());

	QA7->TimerClearReload.IntClear = 1;	// Clear interrupt
	QA7->TimerClearReload.Reload = 1;	// Reload now
}

void map_timer_interupt_core_3()
{
	if(!get_core_id()) return; //Core !0 running this function

	QA7->TimerRouting.Routing = LOCALTIMER_TO_CORE3_IRQ;	// Route local timer IRQ to Core0

	QA7->TimerControlStatus.ReloadValue = 20000000;	// Timer period set
	QA7->TimerControlStatus.TimerEnable = 1;	// Timer enabled
	QA7->TimerControlStatus.IntEnable = 1;		// Timer IRQ enabled

	QA7->TimerClearReload.IntClear = 1;		// Clear interrupt
	QA7->TimerClearReload.Reload = 1;		// Reload now

	QA7->Core3TimerIntControl.nCNTPNSIRQ_IRQ = 1;	// We are in NS EL1 so enable IRQ to core0 that level
	QA7->Core3TimerIntControl.nCNTPNSIRQ_FIQ = 0;	// Make sure FIQ is zero
}

void core_3_enable_interupts_deadloop()
{
	enable_irq();
	while(1) {asm volatile ("nop");}
}

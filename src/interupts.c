//I'm not yet sure how this works or what I'm doing all of this is experimental

void enable_irq()
{
	asm volatile("msr daifclr, #2");
}

void disable_irq()
{
	asm volatile("msr daifset, #2");
}

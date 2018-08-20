SECTIONS
{
	. = 0x80000;
	_start = .;
	
	.text : 
	{
		KEEP(*(.text.boot)) *(.text .text.* .gnu.linkonce.t*) 
	}
	
	.rodata : 
	{ 
		*(.rodata .rodata.* .gnu.linkonce.r*) 
	}
	
	PROVIDE(_data = .);
	.data :
	{ 
		*(.data .data.* .gnu.linkonce.d*) 
	}
	
	.bss (NOLOAD) : 
	{
		. = ALIGN(16);
		__bss_start = .;
		*(.bss .bss.*)
		*(COMMON)
		__bss_end = .;
	}

	.stack_core0 :
	{
		. = ALIGN(16);  /* Stack must always be aligned to 8 byte boundary AAPCS64 call standard */
		__stack_start_core0__ = .;
		. = . + 512;    /* EL0 stack size */
		__EL0_stack_core0 = .;
		. = . + 32768;  /* EL1 stack size */
		__EL1_stack_core0 = .;
		. = . + 4096;  /* EL2 stack size (start-up) */
		__EL2_stack_core0 = .;
		. = ALIGN(16);
		__stack_end_core0__ = .;
	}

	.stack_core1 :
	{
		. = ALIGN(16);  /* Stack must always be aligned to 8 byte boundary AAPCS64 call standard */
		__stack_start_core1__ = .;
		. = . + 512;    /* EL0 stack size */
		__EL0_stack_core1 = .;
		. = . + 32768;    /* EL1 stack size */
		__EL1_stack_core1 = .;
		. = . + 4096;  /* EL2 stack size (start-up) */
		__EL2_stack_core1 = .;
		. = ALIGN(16);
		__stack_end_core1__ = .;
	}

	.stack_core2 :
	{
		. = ALIGN(16);  /* Stack must always be aligned to 8 byte boundary AAPCS call standard */
		__stack_start_core2__ = .;
		. = . + 512;    /* EL0 stack size */
		__EL0_stack_core2 = .;
		. = . + 32768;    /* EL1 stack size */
		__EL1_stack_core2 = .;
		. = . + 4096;  /* EL2 stack size (start-up) */
		__EL2_stack_core2 = .;
		. = ALIGN(16);
		__stack_end_core2__ = .;
	}

	.stack_core3 :
	{
		. = ALIGN(16);  /* Stack must always be aligned to 8 byte boundary AAPCS call standard */
		__stack_start_core3__ = .;
		. = . + 512;    /* EL0 stack size */
		__EL0_stack_core3 = .;
		. = . + 32768;    /* EL1 stack size */
		__EL1_stack_core3 = .;
		. = . + 4096;  /* EL2 stack size (start-up) */
		__EL2_stack_core3 = .;
		. = ALIGN(16);
		__stack_end_core3__ = .;
	}
	
	_end = .;

	/DISCARD/ : { *(.comment) *(.gnu*) *(.note*) *(.eh_frame*) }
}
__bss_size = (__bss_end - __bss_start)>>3;

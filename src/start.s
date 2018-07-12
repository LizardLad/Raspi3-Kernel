.section ".text.boot"
.balign 4
.global _start

_start:	
//"================================================================"
//  Initilize MPID/MPIDR registers for all Cores
//"================================================================"
	mrs	x0, midr_el1
	mrs	x1, mpidr_el1
	msr	vpidr_el2, x0
	msr	vmpidr_el2, x1

//"================================================================"
//  Disable coprocessor traps for all Cores
//"================================================================"
	mov	x0, #0x33ff
	msr	cptr_el2, x0						// Disable coprocessor traps to EL2
	msr	hstr_el2, xzr						// Disable coprocessor traps to EL2
	mov	x0, #3 << 20
	msr	cpacr_el1, x0						// Enable FP/SIMD at EL1

//"================================================================"
//  Enable CNTP for EL1
//"================================================================"
	mrs	x0, cnthctl_el2
	orr	x0, x0, #3
	msr	cnthctl_el2, x0
	msr	cntvoff_el2, xzr

//"================================================================"
//  Initialize HCR_EL2 so EL1 is 64 bits for all Cores
//"================================================================"
	mov	x0, #(1 << 31)						// 64bit EL1
	msr	hcr_el2, x0

//"================================================================"
//  Initialize SCTLR_EL1 for all Cores
//"================================================================"
	/*  RES1 bits (29,28,23,22,20,11) to 1
	 *  RES0 bits (31,30,27,21,17,13,10,6) +
	 *  UCI,EE,EOE,WXN,nTWE,nTWI,UCT,DZE,I,UMA,SED,ITD,
	 *  CP15BEN,SA0,SA,C,A,M to 0 */
	mov	x0, #0x0800
	movk	x0, #0x30d0, lsl #16
	orr    x0, x0, #(0x1 << 2)            // The C bit on (data cache). 
	orr    x0, x0, #(0x1 << 12)           // The I bit on (instruction cache)
	msr	sctlr_el1, x0

//"================================================================"
//  Set up exception handlers
//"================================================================"
	ldr	x2, =_vectors
	msr	vbar_el1, x2

//"================================================================"
//  Return to the EL1_SP1 mode from EL2 for all Cores
//"================================================================"
	mov	x0, #0x3c5							// EL1_SP1 | D | A | I | F
	msr	spsr_el2, x0						// Set spsr_el2 with settings
	adr	x0, exit_el1						// Address to exit EL2
	msr	elr_el2, x0							// Set elevated return register
	eret									// Call elevated return

//"================================================================"
//  Branch all cores to their destination
//"================================================================"
exit_el1:	
	mrs 	x1, mpidr_el1 // Read core id on AARCH64
	and 	x1, x1, #0x3  // Make core 2 bit bitmask in x1
	cbz  	x1, 2f	      // Core 0 jumps out to label 2
	cmp 	x1, #1        // Check for core1
	beq  	1f
	b 	_hang

1:
	ldr     x1, =(_start-0x10000)
	mov     sp, x1
	bl       core1_main
	b _hang

// core0 main
2:	
	ldr     x1, =_start
	mov     sp, x1


//"================================================================"
//  About to go to into C kernel clear BSS (Core0 only)
//"================================================================"
	ldr x3, =__bss_end
	ldr x0, =__bss_start
	cmp	x0, x3
	bcs	main
.bss_zero_loop:
	str	wzr, [x0], 4
	cmp	x3, x0
	bhi	.bss_zero_loop

.global _asm_enter_main
	bl	main

.global _hang
_hang:
	wfe
	b _hang


//Important this code must be properly aligned!
	.align 11
_vectors:
	//Synchronous
	.align 7
	mov x0, #0
	mrs x1, esr_el1
	mrs x2, elr_el1
	mrs x3, spsr_el1
	mrs x4, far_el1
	b exc_handler

	//IRQ
	.align 7
	mov x0, #1
        mrs x1, esr_el1
        mrs x2, elr_el1
        mrs x3, spsr_el1
        mrs x4, far_el1
        b exc_handler

	//FIQ
	.align 7
	mov x0, #2
        mrs x1, esr_el1
        mrs x2, elr_el1
        mrs x3, spsr_el1
        mrs x4, far_el1
        b exc_handler

	//SError
	.align 7
	mov x0, #3
        mrs x1, esr_el1
        mrs x2, elr_el1
        mrs x3, spsr_el1
        mrs x4, far_el1
        b exc_handler

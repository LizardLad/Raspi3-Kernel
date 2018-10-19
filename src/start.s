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
	ldr	x0, =_vectors
	msr	vbar_el1, x0

//"================================================================"
//  Set up stack pointers
//"================================================================"
	mrs 	x1, mpidr_el1	// Read core id on AARCH64
	and 	x1, x1, #0x3	// Make core 2 bit bitmask in x1
	cmp	x1, #0
	beq	Core0StackPointers
	cmp	x1, #1
	beq	Core1StackPointers
	cmp	x1, #2
	beq	Core2StackPointers
	cmp	x1, #3
	beq	Core3StackPointers
	b	_hang

//Core 1 stack pointers
Core1StackPointers:
	ldr	x1, =__EL0_stack_core1
	ldr     x2, =__EL1_stack_core1
	ldr	x3, =__EL2_stack_core1
	msr	sp_el0, x1
	msr     sp_el1, x2
	mov	sp, x3
	b 	EL2_ret

//Core 2 stack pointers
Core2StackPointers:
	ldr	x1, =__EL0_stack_core2
	ldr	x2, =__EL1_stack_core2
	ldr	x3, =__EL2_stack_core2
	msr	sp_el0, x1
	msr     sp_el1, x2
	mov	sp, x3
	b 	EL2_ret

//Core 3 stack pointers
Core3StackPointers:
	ldr	x1, =__EL0_stack_core3
	ldr	x2, =__EL1_stack_core3
	ldr	x3, =__EL2_stack_core3
	msr	sp_el0, x1
	msr     sp_el1, x2
	mov	sp, x3
	b	EL2_ret

//Core 0 stack pointers
Core0StackPointers:
	ldr	x1, =__EL0_stack_core0
	ldr     x2, =__EL1_stack_core0
	ldr	x3, =__EL2_stack_core0
	msr	sp_el0, x1
	msr     sp_el1, x2
	mov	sp, x3
	b	EL2_ret

//"================================================================"
//  Return to the EL1_SP1 mode from EL2 for all Cores
//"================================================================"
EL2_ret:
	mov	x0, #0x3c5					// EL1_SP1 | D | A | I | F
	msr	spsr_el2, x0					// Set spsr_el2 with settings
	adr	x0, exit_el1					// Address to exit EL2
	msr	elr_el2, x0					// Set elevated return register
	eret							// Call elevated return

//"================================================================"
//  Branch all cores to their destination
//"================================================================"
exit_el1:	
	mrs 	x1, mpidr_el1	// Read core id on AARCH64
	and 	x1, x1, #0x3	// Make core 2 bit bitmask in x1
	cbz  	x1, 2f		// Core 0 jumps out to label 2
	cmp 	x1, #1		// Check for core1
	beq  	1f		//Core 1 jumps out to lable 1
	cmp	x1, #2		//Check for Core 2
	beq	1f		//Core 2 jump to label 2
	cmp	x1, #3		//Check for Core 3
	beq	1f		//Core 3 jump to label 3
	b 	_hang		//I don't know why this would actually happen

//Core 1, 2, 3 main
1:
	bl	core_wait_for_instruction
	b	_hang

//Core 0 main
2:
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


// save registers before we call any C code
dbg_saveregs:
	ldr     x29, =dbg_regs     // We saved this register before call so fine to use it
	str     x0, [x29], #8
	str     x1, [x29], #8
	str     x2, [x29], #8        // dbg_regs[2]=x2
	str     x3, [x29], #8        // ...etc.
	str     x4, [x29], #8
	str     x5, [x29], #8
	str     x6, [x29], #8
	str     x7, [x29], #8
	str     x8, [x29], #8
	str     x9, [x29], #8
	str     x10, [x29], #8
	str     x11, [x29], #8
	str     x12, [x29], #8
	str     x13, [x29], #8
	str     x14, [x29], #8
	str     x15, [x29], #8
	str     x16, [x29], #8
	str     x17, [x29], #8
	str     x18, [x29], #8
	str     x19, [x29], #8
	str     x20, [x29], #8
	str     x21, [x29], #8
	str     x22, [x29], #8
	str     x23, [x29], #8
	str     x24, [x29], #8
	str     x25, [x29], #8
	str     x26, [x29], #8 
	str     x27, [x29], #8
	str     x28, [x29], #8
	/* remember we pushed r29, r30 at irq start */
	mov	x0, sp            // current stack position
	mov     x2, #(32*8)       // that is where stack was when we saved them (32registers * 8 bytes).. I think needs checking
	add     x2,  x2, x0       // Now add current stack position
	ldp	x0, x1, [x2], #0  // If I am right that is now original x29,x30 in x0, x1
	str     x0, [x29], #8
	str     x1, [x29], #8
   // also read and store some system registers
	mrs     x1, elr_el1
	str     x1, [x29], #8
	mrs     x1, spsr_el1
	str     x1, [x29], #8
	mrs     x1, esr_el1
	str     x1, [x29], #8
	mrs     x1, far_el1
	str     x1, [x29], #8
	mrs     x1, sctlr_el1
	str     x1, [x29], #8
	mrs     x1, tcr_el1
	str     x1, [x29], #8
	ret

// Save all corruptible registers (x29,x30 are assumed done before call)
register_save:
	stp	x27, x28, [sp, #-16]!
	stp	x25, x26, [sp, #-16]!
	stp	x23, x24, [sp, #-16]!
	stp	x21, x22, [sp, #-16]!
	stp	x19, x20, [sp, #-16]!
	stp	x17, x18, [sp, #-16]!
	stp	x15, x16, [sp, #-16]!
	stp	x13, x14, [sp, #-16]!
	stp	x11, x12, [sp, #-16]!
	stp	x9, x10, [sp, #-16]!
	stp	x7, x8, [sp, #-16]!
	stp	x5, x6, [sp, #-16]!
	stp	x3, x4, [sp, #-16]!
	stp	x1, x2, [sp, #-16]!
	str	x0, [sp, #-16]!
	ret

// Restore all corruptible registers
register_restore:
	ldr	x0, [sp], #16
	ldp	x1, x2, [sp], #16
	ldp	x3, x4, [sp], #16
	ldp	x5, x6, [sp], #16
	ldp	x7, x8, [sp], #16
	ldp	x9, x10, [sp], #16
	ldp	x11, x12, [sp], #16
	ldp	x13, x14, [sp], #16
	ldp	x15, x16, [sp], #16
	ldp	x17, x18, [sp], #16
	ldp	x19, x20, [sp], #16
	ldp	x21, x22, [sp], #16
	ldp	x23, x24, [sp], #16
	ldp	x25, x26, [sp], #16
	ldp	x27, x28, [sp], #16
	ret

	// important, code has to be properly aligned
	.balign 0x800
_vectors:
	/* from current EL with sp_el0 */
	// synchronous
	.balign 0x80
	b _hang

	// IRQ
	.balign 0x80
	b _hang	

	// FIQ
	.balign 0x80
	b _hang	

	// SError
	.balign 0x80
	b _hang	

	/* from current EL with sp_elx, x != 0 */
	// synchronous
	.balign 0x80
	stp	x29, x30, [sp, #-16]!	 // Save x30 link register and x29 just so we dont waste space
	bl	register_save		 // Save corruptible registers .. it assumes x29,x30 saved
	bl      dbg_saveregs
	mov     x0, #0
	bl      dbg_decodeexc
	bl      dbg_main
	bl	register_restore	// restore corruptible registers .. does all but x29,x30
	ldp	x29, x30, [sp], #16		// restore x29,x30 pulling stack back up 16
	eret

	// IRQ
	.balign  0x80
	stp	x29, x30, [sp, #-16]!	 // Save x30 link register and x29
	bl      dbg_saveregs
	//mov     x0, #1
	//bl      dbg_decodeexc
	bl      c_irq_handler		//dbg_main is now only called if the IRQ didn't come from something that was set to deliver interupts
	bl	register_restore	// restore corruptible registers .. does all but x29,x30
	ldp	x29, x30, [sp], #16		// restore x29,x30 pulling stack back up 16
	eret

	// FIQ
	.balign  0x80
	stp	x29, x30, [sp, #-16]!	 // Save x30 link register and x29 just so we dont waste space
	bl	register_save		 // Save corruptible registers .. it assumes x29,x30 saved
	bl      dbg_saveregs
	mov     x0, #2
	bl      dbg_decodeexc
	bl      dbg_main
	bl	register_restore	// restore corruptible registers .. does all but x29,x30
	ldp	x29, x30, [sp], #16		// restore x29,x30 pulling stack back up 16
	eret

	// SError
	.balign  0x80
	stp	x29, x30, [sp, #-16]!	 // Save x30 link register and x29 just so we dont waste space
	bl	register_save		 // Save corruptible registers .. it assumes x29,x30 saved
	bl      dbg_saveregs
	mov     x0, #3
	bl      dbg_decodeexc
	bl      dbg_main
	bl	register_restore	// restore corruptible registers .. does all but x29,x30
	ldp	x29, x30, [sp], #16		// restore x29,x30 pulling stack back up 16
	eret
	
	/* from lower EL, target minus 1 is AARCH64 */
	// synchronous
	.balign 0x80
	b _hang

	// IRQ
	.balign 0x80
	b _hang	

	// FIQ
	.balign 0x80
	b _hang	

	// SError
	.balign 0x80
	b _hang	

	/* from lower EL, target minus 1 is AARCH32 */
	// synchronous
	.balign 0x80
	b _hang

	// IRQ
	.balign 0x80
	b _hang	

	// FIQ
	.balign 0x80
	b _hang	

	// SError
	.balign 0x80
	b _hang	

//"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
//				                 SEMAPHORE ROUTINES		    
//"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"

//"========================================================================="
//	semaphore_inc -- AARCH64 Pi3 code
//	C Function: "void semaphore_inc (uint32_t* sem);"
//	Entry: X0 will have semaphore address value
//	Return: nothing
//"========================================================================="
.section .text.semaphore_inc, "ax", %progbits
.balign	4
.globl semaphore_inc;
.type semaphore_inc, %function
semaphore_inc:
      mov   w2, 1
	.p2align 2
.lockloop:
    ldaxr   w1, [x0]
    stxr    w3, w2, [x0]
    cbnz    w3, .lockloop
    cbnz    w1, .lockloop
	dmb ish
    ret
.balign	4
.ltorg										// Tell assembler ltorg data for this code can go here
.size	semaphore_inc, .-semaphore_inc

//"========================================================================="
//	semaphore_dec -- AARCH64 Pi3 code
//	C Function: "void semaphore_dec (uint32_t* sem);"
//	Entry: X0 will have semaphore address value
//	Return: nothing
//"========================================================================="
.section .text.semaphore_dec, "ax", %progbits
.balign	4
.globl semaphore_dec;
.type semaphore_dec, %function
semaphore_dec:
	stlrb	wzr, [x0]
	dmb ish
	ret
.balign	4
.ltorg										// Tell assembler ltorg data for this code can go here
.size	semaphore_dec, .-semaphore_dec

/* "PROVIDE C FUNCTION: uint32_t ARM_addr_to_GPU_addr (void* ARMaddress);" */
.section .text.ARMaddrToGPUaddr, "ax", %progbits
.balign	4
.globl ARM_addr_to_GPU_addr;		
.type ARM_addr_to_GPU_addr, %function
//"================================================================"
// ARMaddrToGPUaddr -- AARCH64 Pi3 code
// C Function: uint32_t ARM_addr_to_GPU_addr (void* ARM_address);
// Entry: x0 will have ARMAddress value
//"================================================================"
ARM_addr_to_GPU_addr:
	mov x1, #0xC0000000						// ARM to VC conversion value
	orr x0, x0, x1							// Create bus address
	ret										// Return
.balign	4
.ltorg										// Tell assembler ltorg data for this code can go here
.size	ARM_addr_to_GPU_addr, .-ARM_addr_to_GPU_addr


/* "PROVIDE C FUNCTION: uint32_t GPU_addr_to_ARM_addr (uint32_t BUSaddress);" */
.section .text.GPUaddrToARMaddr, "ax", %progbits
.balign	4
.globl GPU_addr_to_ARM_addr;		
.type GPU_addr_to_ARM_addr, %function
//"================================================================"
// GPUaddrToARMaddr -- AARCH64 Pi3 code
// C Function: uint32_t GPUaddrToARMaddr (uint32_t BUS_address);
// Entry: x0 will have GPUAddress value
//"================================================================"
GPU_addr_to_ARM_addr:
	mov x1, #0xC0000000						// ARM to VC conversion value	
	bic x0, x0, x1							// Create arm address
	ret										// Return
.balign	4
.ltorg										// Tell assembler ltorg data for this code can go here
.size	GPU_addr_to_ARM_addr, .-GPU_addr_to_ARM_addr

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

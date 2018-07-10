	.arch armv8-a
	.file	"sound.c"
	.comm	master_boot_record_array,512,4
	.comm	partitionlba,4,4
	.comm	EndOfProgram,8,8
	.comm	TotalSpaceAvaliable,8,8
	.comm	ChunkSize,8,8
	.comm	NumberOfChunks,8,8
	.section	.rodata
	.align	3
.LC0:
	.string	"Well there were no unaligned exeptions"
	.text
	.align	2
	.global	init_audio_jack
	.type	init_audio_jack, %function
init_audio_jack:
.LFB11:
	.cfi_startproc
	stp	x29, x30, [sp, -16]!
	.cfi_def_cfa_offset 16
	.cfi_offset 29, -16
	.cfi_offset 30, -8
	add	x29, sp, 0
	.cfi_def_cfa_register 29
	mov	x0, 16
	movk	x0, 0x3f20, lsl 16
	mov	w1, 4
	movk	w1, 0x20, lsl 16
	str	w1, [x0]
	mov	x0, 4752
	movk	x0, 0x3f10, lsl 16
	mov	w1, 8192
	movk	w1, 0x5a00, lsl 16
	str	w1, [x0]
	mov	x0, 4736
	movk	x0, 0x3f10, lsl 16
	mov	w1, 22
	movk	w1, 0x5a00, lsl 16
	str	w1, [x0]
	mov	x0, 49216
	movk	x0, 0x3f20, lsl 16
	mov	w1, 11336
	str	w1, [x0]
	mov	x0, 49280
	movk	x0, 0x3f20, lsl 16
	mov	w1, 11336
	str	w1, [x0]
	mov	x0, 49152
	movk	x0, 0x3f20, lsl 16
	mov	w1, 8545
	str	w1, [x0]
	adrp	x0, .LC0
	add	x0, x0, :lo12:.LC0
	mov	x2, x0
	mov	w1, 2
	mov	w0, 0
	bl	lfb_print
	mov	w0, 0
	ldp	x29, x30, [sp], 16
	.cfi_restore 30
	.cfi_restore 29
	.cfi_def_cfa 31, 0
	ret
	.cfi_endproc
.LFE11:
	.size	init_audio_jack, .-init_audio_jack
	.section	.rodata
	.align	3
.LC1:
	.string	"End isn't less than start."
	.align	3
.LC2:
	.string	"Is a multiple of two so it is 16bit"
	.align	3
.LC3:
	.string	"Completed Audio"
	.text
	.align	2
	.global	play_16bit_unsigned_audio
	.type	play_16bit_unsigned_audio, %function
play_16bit_unsigned_audio:
.LFB12:
	.cfi_startproc
	stp	x29, x30, [sp, -48]!
	.cfi_def_cfa_offset 48
	.cfi_offset 29, -48
	.cfi_offset 30, -40
	add	x29, sp, 0
	.cfi_def_cfa_register 29
	str	x0, [x29, 24]
	str	x1, [x29, 16]
	ldr	x1, [x29, 16]
	ldr	x0, [x29, 24]
	cmp	x1, x0
	bcs	.L4
	mov	w0, 1
	b	.L5
.L4:
	adrp	x0, .LC1
	add	x0, x0, :lo12:.LC1
	mov	x2, x0
	mov	w1, 3
	mov	w0, 0
	bl	lfb_print
	ldr	x1, [x29, 24]
	ldr	x0, [x29, 16]
	sub	x0, x1, x0
	and	x0, x0, 1
	cmp	x0, 0
	beq	.L6
	mov	w0, 2
	b	.L5
.L6:
	adrp	x0, .LC2
	add	x0, x0, :lo12:.LC2
	mov	x2, x0
	mov	w1, 4
	mov	w0, 0
	bl	lfb_print
	str	wzr, [x29, 44]
	b	.L7
.L9:
	ldrsw	x0, [x29, 44]
	ldr	x1, [x29, 24]
	add	x0, x1, x0
	ldrb	w0, [x0]
	strb	w0, [x29, 43]
	ldr	w0, [x29, 44]
	add	w1, w0, 1
	str	w1, [x29, 44]
	sxtw	x0, w0
	ldr	x1, [x29, 24]
	add	x0, x1, x0
	ldrb	w0, [x0]
	strb	w0, [x29, 42]
	ldrb	w0, [x29, 43]
	sxth	w1, w0
	ldrb	w0, [x29, 42]
	lsl	w0, w0, 8
	sxth	w0, w0
	orr	w0, w1, w0
	sxth	w0, w0
	strh	w0, [x29, 40]
	ldrh	w0, [x29, 40]
	lsr	w0, w0, 2
	strh	w0, [x29, 40]
	mov	x0, 49248
	movk	x0, 0x3f20, lsl 16
	ldrh	w1, [x29, 40]
	str	w1, [x0]
	nop
.L8:
	mov	x0, 49168
	movk	x0, 0x3f20, lsl 16
	ldr	w0, [x0]
	cmp	w0, 1
	bne	.L8
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L7:
	ldrsw	x0, [x29, 44]
	ldr	x1, [x29, 24]
	add	x0, x1, x0
	ldr	x1, [x29, 16]
	cmp	x1, x0
	bne	.L9
	adrp	x0, .LC3
	add	x0, x0, :lo12:.LC3
	mov	x2, x0
	mov	w1, 5
	mov	w0, 0
	bl	lfb_print
	mov	w0, 0
.L5:
	ldp	x29, x30, [sp], 48
	.cfi_restore 30
	.cfi_restore 29
	.cfi_def_cfa 31, 0
	ret
	.cfi_endproc
.LFE12:
	.size	play_16bit_unsigned_audio, .-play_16bit_unsigned_audio
	.ident	"GCC: (GNU) 7.2.1 20170915 (Red Hat Cross 7.2.1-1)"
	.section	.note.GNU-stack,"",@progbits

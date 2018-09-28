// Bus Address
PERIPHERAL_BASE		=	0x3F000000 // Peripheral Base Address

// CM / Clock Manager
CM_BASE   		=	0x101000 // Clock Manager Base Address
CM_PWMDIV		=	0x0A4 // Clock Manager PWM Clock Divisor
CM_SRC_OSCILLATOR	=	0x01 // Clock Control: Clock Source = Oscillator
CM_SRC_PLLCPER		=	0x05 // Clock Control: Clock Source = PLLC Per
CM_ENAB			=	0x10 // Clock Control: Enable The Clock Generator
CM_PASSWORD		=	0x5A000000 // Clock Control: Password "5A"
CM_PWMCTL		=	0x0A0 // Clock Manager PWM Clock Control

// GPIO
GPIO_BASE		=	0x200000 // GPIO Base Address
GPIO_GPFSEL4		=	0x10 // GPIO Function Select 4
GPIO_FSEL0_ALT0		=	0x4 // GPIO Function Select: GPIO Pin X0 Takes Alternate Function 0
GPIO_FSEL5_ALT0		=	0x20000 // GPIO Function Select: GPIO Pin X5 Takes Alternate Function 0

// PWM / Pulse Width Modulator Interface
PWM_BASE	=	0x20C000 // PWM Base Address
PWM_CTL		=	0x0 // PWM Control
PWM_STA		=	0x4 // PWM Status
PWM_RNG1	=	0x10 // PWM Channel 1 Range
PWM_FIF1	=	0x18 // PWM FIFO Input
PWM_RNG2	=	0x20 // PWM Channel 2 Range
PWM_PWEN1	=	0x1 // PWM Control: Channel 1 Enable
PWM_USEF1	=	0x20 // PWM Control: Channel 1 Use Fifo
PWM_CLRF1	=	0x40 // PWM Control: Clear Fifo
PWM_PWEN2	=	0x100 // PWM Control: Channel 2 Enable
PWM_USEF2	=	0x2000 // PWM Control: Channel 2 Use Fifo
PWM_FULL1	=	0x1 // PWM Status: Fifo Full Flag

.section .text.init_audio_jack, "ax", %progbits
.balign	4
.globl init_audio_jack;
.type init_audio_jack, %function
init_audio_jack:
	mov x0,PERIPHERAL_BASE + GPIO_BASE
	mov w1,GPIO_FSEL0_ALT0
	orr w1,w1,GPIO_FSEL5_ALT0
	str w1,[x0,GPIO_GPFSEL4]

	// Set Clock
	mov w0,(PERIPHERAL_BASE + CM_BASE) & 0x0000FFFF
	mov w1,(PERIPHERAL_BASE + CM_BASE) & 0xFFFF0000
	orr w0,w0,w1
	mov w1,CM_PASSWORD
	orr w1,w1,0x2000 // Bits 0..11 Fractional Part Of Divisor = 0, Bits 12..23 Integer Part Of Divisor = 2
	str w1,[x0,CM_PWMDIV]

	mov w1,CM_PASSWORD
	orr w1,w1,CM_ENAB
	orr w1,w1,CM_SRC_OSCILLATOR + CM_SRC_PLLCPER // Use 650MHz PLLC Clock
	str w1,[x0,CM_PWMCTL]

	// Set PWM
	mov w0,(PERIPHERAL_BASE + PWM_BASE) & 0x0000FFFF
	mov w1,(PERIPHERAL_BASE + PWM_BASE) & 0xFFFF0000
	orr w0,w0,w1
	mov w1,0x1624 // Range = 13bit 44100Hz Mono
	str w1,[x0,PWM_RNG1]
	str w1,[x0,PWM_RNG2]

	mov w1,PWM_USEF2 + PWM_PWEN2 + PWM_USEF1 + PWM_PWEN1 + PWM_CLRF1
	str w1,[x0,PWM_CTL]
	ret

.section .text.play_audio, "ax", %progbits
.balign	4
.globl play_audio;
.type play_audio, %function
play_audio:
	mov x0,PERIPHERAL_BASE + GPIO_BASE
	mov w0,(PERIPHERAL_BASE + PWM_BASE) & 0x0000FFFF
	mov w1,(PERIPHERAL_BASE + PWM_BASE) & 0xFFFF0000
	orr w0,w0,w1

	Loop:
		adr x1, _binary_src_audio_Interlude_bin_start // X1 = Sound Sample
		ldr w2, =_binary_src_audio_Interlude_bin_end
		and w2, w2, 0x0000FFFF // W2 = End Of Sound Sample
		ldr w3, =_binary_src_audio_Interlude_bin_end
		and w3, w3, 0xFFFF0000
		orr w2,w2,w3
		FIFO_Write:
			ldrh w3,[x1],2 // Write 2 Bytes To FIFO
			lsr w3,w3,4 // Convert 16bit To 12bit
			str w3,[x0,PWM_FIF1] // FIFO Address
			
			ldrh w3, [x1], 2
			lsr w3, w3, 4
			str w3, [x0, PWM_FIF1]
		FIFO_Wait:
			ldr w3,[x0,PWM_STA]
			tst w3,PWM_FULL1 // Test Bit 1 FIFO Full
			b.ne FIFO_Wait
		cmp w1,w2 // Check End Of Sound Sample
		b.ne FIFO_Write
	b Loop // Play Sample Again

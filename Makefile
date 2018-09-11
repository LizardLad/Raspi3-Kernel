SOURCE = src/
NONOPTIMISED = src/NoOptimisation/
BUILD = build/
TARGET = kernel8.img
LINKER = link.ld
FONT = src/font/font.psf
FONTOBJ = build/font.o
AUDIO = src/audio/Interlude.bin
AUDIOOBJ = build/Interlude.o
LIBS = lib/libopenlibm.a
LIBOBJS = build/libopenlibm.o
ASMOBJS = $(patsubst $(SOURCE)%.s,$(BUILD)%.o,$(wildcard $(SOURCE)*.s))
COBJS = $(patsubst $(SOURCE)%.c,$(BUILD)%.o,$(wildcard $(SOURCE)*.c))
COBJSNOOP = $(patsubst $(NONOPTIMISED)%.c,$(BUILD)%.o,$(wildcard $(NONOPTIMISED)*.c))

CFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -nostartfiles -std=gnu99 -mstrict-align
CFLAGSNOOP = -Wall -ffreestanding -nostdinc -nostdlib -nostartfiles

all: clean kernel8.img

$(BUILD)%.o: $(SOURCE)%.s
	aarch64-linux-gnu-gcc -g $(CFLAGS) -c  $< -o $@

$(BUILD)%.o: $(SOURCE)%.c
	aarch64-linux-gnu-gcc -g $(CFLAGS) -c  $< -o $@  

$(FONTOBJ): $(FONT)
	aarch64-linux-gnu-ld -r -b binary -o $(FONTOBJ) $(FONT)

$(AUDIOOBJ): $(AUDIO)
	aarch64-linux-gnu-ld -r -b binary -o $(AUDIOOBJ) $(AUDIO)
	
$(BUILD)%.o: $(NONOPTIMISED)%.c
	aarch64-linux-gnu-gcc $(CFLAGSNOOP) -c $< -o $@


kernel8.img: $(ASMOBJS) $(COBJS) $(FONTOBJ) $(COBJSNOOP) $(AUDIOOBJ)
	aarch64-linux-gnu-ld -nostdlib -nostartfiles $(FONTOBJ) $(AUDIOOBJ) $(ASMOBJS) $(COBJSNOOP) $(COBJS) -L lib -l openlibm -T $(LINKER) -o $(BUILD)kernel.elf
	aarch64-linux-gnu-objcopy $(BUILD)kernel.elf -O binary $(TARGET)

clean:
	rm -rf build/* 
	rm -rf $(TARGET)

run:
	/home/Oliver/Development/qemu/aarch64-softmmu/qemu-system-aarch64 -M raspi3 -kernel kernel8.img -drive file=raspi.iso,if=sd,format=raw -serial stdio -d int,in_asm

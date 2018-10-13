SOURCE = src/
COMPILER_ROOT = ../Linaro\ Cross\ Compiler/bin/aarch64-elf-
BUILD = build/
TARGET = kernel8.img
LINKER_SCRIPT = link.ld
FONT = src/font/font.psf
FONTOBJ = build/font.o
ASMOBJS = $(patsubst $(SOURCE)%.s,$(BUILD)%.o,$(wildcard $(SOURCE)*.s))
COBJS = $(patsubst $(SOURCE)%.c,$(BUILD)%.o,$(wildcard $(SOURCE)*.c))

CFLAGS = -Wall -O2 -ffreestanding -nostartfiles -std=gnu11 -mstrict-align -mcpu=cortex-a53 -fno-tree-loop-vectorize -fno-tree-slp-vectorize -g -lm -lc -lgcc

all: clean kernel8.img

$(BUILD)%.o: $(SOURCE)%.s
	$(COMPILER_ROOT)gcc $(CFLAGS) -c  $< -o $@

$(BUILD)%.o: $(SOURCE)%.c
	$(COMPILER_ROOT)gcc $(CFLAGS) -c  $< -o $@  

$(FONTOBJ): $(FONT)
	$(COMPILER_ROOT)ld -r -b binary -o $(FONTOBJ) $(FONT)

kernel8.img: $(ASMOBJS) $(COBJS) $(FONTOBJ) $(COBJSNOOP)
	$(COMPILER_ROOT)gcc -nostartfiles $(FONTOBJ) $(ASMOBJS) $(COBJS) -T $(LINKER_SCRIPT) -o $(BUILD)kernel.elf -lm -lc -lgcc
	$(COMPILER_ROOT)objcopy $(BUILD)kernel.elf -O binary $(TARGET)

clean:
	rm -rf build/* 
	rm -rf $(TARGET)

run:
	/home/Oliver/Development/qemu/aarch64-softmmu/qemu-system-aarch64 -M raspi3 -kernel kernel8.img -drive file=raspi.iso,if=sd,format=raw -serial stdio -d int,in_asm > qemu-output.info

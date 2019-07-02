SOURCE = src/
FAT_DRIVER_SOURCE = src/fat_driver/
COMPILER_ROOT = ../Linaro\ Cross\ Compiler/bin/aarch64-elf-
BUILD = build/
FAT_BUILD = build/fat/
TARGET = kernel8.img
LINKER_SCRIPT = link.ld
FONT = src/font/font.psf
FONTOBJ = build/font.o
ASMOBJS = $(patsubst $(SOURCE)%.s,$(BUILD)%.o,$(wildcard $(SOURCE)*.s))
COBJS = $(patsubst $(SOURCE)%.c,$(BUILD)%.o,$(wildcard $(SOURCE)*.c))
FAT_COBJS = $(patsubst $(FAT_DRIVER_SOURCE)%.c,$(FAT_BUILD)%.o,$(wildcard $(FAT_DRIVER_SOURCE)*.c))

CFLAGS = -Wall -O2 -ffreestanding -nostartfiles -std=gnu11 -mstrict-align -mcpu=cortex-a53 -fno-tree-loop-vectorize -fno-tree-slp-vectorize -g -lm -lc -lgcc

all: kernel8.img

$(BUILD)%.o: $(SOURCE)%.s
	$(COMPILER_ROOT)gcc $(CFLAGS) -c  $< -o $@

$(BUILD)%.o: $(SOURCE)%.c
	$(COMPILER_ROOT)gcc $(CFLAGS) -c  $< -o $@ 

$(FAT_BUILD)%.o: $(FAT_DRIVER_SOURCE)%.c
	$(COMPILER_ROOT)gcc $(CFLAGS) -c  $< -o $@

$(FONTOBJ): $(FONT)
	$(COMPILER_ROOT)ld -r -b binary -o $(FONTOBJ) $(FONT)

kernel8.img: $(FAT_COBJS) $(ASMOBJS) $(COBJS) $(FONTOBJ)
	$(COMPILER_ROOT)gcc -nostartfiles $(FONTOBJ) $(ASMOBJS) $(COBJS) $(FAT_COBJS) -T $(LINKER_SCRIPT) -o $(BUILD)kernel.elf -lm -lc -lgcc
	$(COMPILER_ROOT)objcopy $(BUILD)kernel.elf -O binary $(TARGET)

clean:
	rm -rf build/*
	rm -rf $(TARGET)
	mkdir build/fat/

run_debug:
	qemu-system-aarch64 -M raspi3 -kernel kernel8.img -drive file=raspi.iso,if=sd,format=raw -serial stdio -d int,in_asm > qemu-output.info

run_gdb:
	qemu-system-aarch64 -M raspi3 -kernel kernel8.img -drive file=raspi.iso,if=sd,format=raw -serial stdio -s -S

run:
	qemu-system-aarch64 -M raspi3 -kernel kernel8.img -drive file=raspi.iso,if=sd,format=raw -serial stdio

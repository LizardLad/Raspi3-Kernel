SOURCE = src/
NONOPTIMISED = src/NoOptimisation/
BUILD = build/
TARGET = kernel8.img
LINKER = link.ld
FONT = src/font/font.psf
FONTOBJ = build/font.o
LIBS = lib/libopenlibm.a
LIBOBJS = build/libopenlibm.o
ASMOBJS = $(patsubst $(SOURCE)%.s,$(BUILD)%.o,$(wildcard $(SOURCE)*.s))
COBJS = $(patsubst $(SOURCE)%.c,$(BUILD)%.o,$(wildcard $(SOURCE)*.c))
COBJSNOOP = $(patsubst $(NONOPTIMISED)%.c,$(BUILD)%.o,$(wildcard $(NONOPTIMISED)*.c))

CFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -nostartfiles
CFLAGSNOOP = -Wall -ffreestanding -nostdinc -nostdlib -nostartfiles

all: clean kernel8.img

$(BUILD)%.o: $(SOURCE)%.s
	aarch64-linux-gnu-gcc -g $(CFLAGS) -c  $< -o $@

$(BUILD)%.o: $(SOURCE)%.c
	aarch64-linux-gnu-gcc -g $(CFLAGS) -c  $< -o $@  

$(FONTOBJ): $(FONT)
	aarch64-linux-gnu-ld -r -b binary -o $(FONTOBJ) $(FONT)
	
$(BUILD)%.o: $(NONOPTIMISED)%.c
	aarch64-linux-gnu-gcc $(CFLAGSNOOP) -c $< -o $@


kernel8.img: $(ASMOBJS) $(COBJS) $(FONTOBJ) $(COBJSNOOP)
	aarch64-linux-gnu-ld -nostdlib -nostartfiles $(FONTOBJ) $(ASMOBJS) $(COBJSNOOP) $(COBJS) -L lib -l openlibm -T $(LINKER) -o $(BUILD)kernel.elf
	aarch64-linux-gnu-objcopy $(BUILD)kernel.elf -O binary $(TARGET)

clean:
	rm -rf build/* 
	rm -rf $(TARGET)

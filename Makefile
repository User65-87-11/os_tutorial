
BUILD_DIR = build
TEMP_DIR = temp
KERNEL_DIR = kernel
DRIVER_DIR = drivers
ASM_DIR = asm
CPU_DIR = cpu
LIBC_DIR = libc
ZERO_FILE = build/zero_mb.dat

BLD_KRN_DIR = build/kernel
BLD_DRV_DIR = build/drivers
BLD_CPU_DIR = build/cpu
BLD_ASM_DIR = build/asm
BLD_LIBC_DIR = build/libc

CFLAGS = -g -std=c23 -m32
LD_FLAGS = -m elf_i386 


objects= $(BUILD_DIR)/kernel_entry.o \
		$(BLD_KRN_DIR)/kernel.o $(BLD_KRN_DIR)/test.o \
		$(BLD_DRV_DIR)/ports.o $(BLD_DRV_DIR)/keyboard.o \
		$(BLD_LIBC_DIR)/32bit-mem.o\
		$(BLD_CPU_DIR)/idt.o $(BLD_CPU_DIR)/isr.o $(BLD_CPU_DIR)/interrupt.o $(BLD_CPU_DIR)/timer.o 


.SILENT:make_objects

#---------------


 

all: echos  init  build_all
	
	
build_all: $(BUILD_DIR)/bootsect.bin $(BUILD_DIR)/kernel.bin $(ZERO_FILE)
	cat $^ > $(BUILD_DIR)/os-image.bin

echos:
	@echo "-----------BUILD------------"

clean:
	@echo "-----------CLEAN------------"
	rm -rf $(BUILD_DIR)/*

init:
	@echo "-----------INIT------------"
	mkdir -p $(BLD_CPU_DIR)
	mkdir -p $(BLD_DRV_DIR)
	mkdir -p $(BLD_ASM_DIR)
	mkdir -p $(BLD_KRN_DIR)
	mkdir -p $(BLD_LIBC_DIR)
	dd if=/dev/zero of=$(ZERO_FILE) bs=1MB count=1



$(BLD_KRN_DIR)/kernel.o: $(KERNEL_DIR)/kernel.c
	gcc $(CFLAGS) -ffreestanding -fno-pie -c $^ -o $@

$(BLD_KRN_DIR)/test.o: $(KERNEL_DIR)/test.c
	gcc  $(CFLAGS) -ffreestanding -fno-pie -c $^ -o $@

$(BLD_DRV_DIR)/ports.o: $(DRIVER_DIR)/ports.c
	gcc  $(CFLAGS) -ffreestanding -fno-pie -c $^ -o $@

$(BLD_DRV_DIR)/keyboard.o: $(DRIVER_DIR)/keyboard.c
	gcc  $(CFLAGS) -ffreestanding -fno-pie -c $^ -o $@


# --- 
$(BLD_CPU_DIR)/timer.o: $(CPU_DIR)/timer.c
	gcc  $(CFLAGS) -ffreestanding -fno-pie -c $^ -o $@

$(BLD_CPU_DIR)/idt.o: $(CPU_DIR)/idt.c
	gcc  $(CFLAGS) -ffreestanding -fno-pie -c $^ -o $@

$(BLD_CPU_DIR)/isr.o : $(CPU_DIR)/isr.c
	gcc  $(CFLAGS) -ffreestanding -fno-pie -c $^ -o $@

$(BLD_CPU_DIR)/interrupt.o: $(CPU_DIR)/interrupt.asm
	nasm $^ -f elf32 -o $@ 	

# ------- asm inc  bin

# incs: $(BUILD_DIR)/kernel/inc.bin


# $(BUILD_DIR)/kernel/inc.bin: $(KERNEL_DIR)/inc.asm
# 	nasm $^ -f bin -o $@

# -------asm
$(BUILD_DIR)/bootsect.bin: $(ASM_DIR)/boot_sect_main.asm
	nasm $^ -f bin -o $@

$(BUILD_DIR)/kernel_entry.o: $(ASM_DIR)/kernel_entry.asm
	nasm $^ -f elf32 -o $@ 

$(BLD_LIBC_DIR)/32bit-mem.o: $(LIBC_DIR)/32bit-mem.asm
	nasm $^ -f elf32 -o $@ 



# -------

$(BUILD_DIR)/kernel.bin: $(objects)
	ld  -Ttext 0x1000  $(LD_FLAGS) --oformat binary  -o $@ $^ 

$(BUILD_DIR)/kernel.elf: $(objects)
	ld -Ttext 0x1000  $(LD_FLAGS) -o $@ $^  


elf: $(BUILD_DIR)/kernel.elf
	@echo "Make elf"

debug: $(BUILD_DIR)/os-image.bin $(BUILD_DIR)/kernel.elf
	gdb	-ex "target remote 172.26.112.1:1234" -ex "symbol-file $(BUILD_DIR)/kernel.elf"

rebuild:init clean 
	@make


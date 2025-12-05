
BUILD_DIR = build
TEMP_DIR = temp
KERNEL_DIR = kernel
DRIVER_DIR = drivers
ASM_DIR = asm
CPU_DIR = cpu
ZERO_FILE = build/zero_mb.dat

BK_DIR = build/kernel
BD_DIR = build/drivers
BCPU_DIR = build/cpu
BA_DIR = build/asm
CFLAGS = -g -std=c23 -m32
LD_FLAGS = -m elf_i386 


objects= $(BUILD_DIR)/kernel_entry.o \
		$(BK_DIR)/kernel.o \
		$(BK_DIR)/test.o \
		$(BD_DIR)/ports.o $(BD_DIR)/keyboard.o \
		$(BUILD_DIR)/kernel/32bit-mem.o\
		$(BCPU_DIR)/idt.o $(BCPU_DIR)/isr.o $(BCPU_DIR)/interrupt.o $(BCPU_DIR)/timer.o 


.SILENT:make_objects

#---------------


 

all: echos init incs build_all
	
	
build_all: $(BUILD_DIR)/bootsect.bin $(BUILD_DIR)/kernel.bin $(ZERO_FILE)
	cat $^ > $(BUILD_DIR)/os-image.bin

echos:
	@echo "-----------BUILD------------"

clean:
	@echo "-----------CLEAN------------"
	rm -rf $(BUILD_DIR)/*

init:
	@echo "-----------INIT------------"
	mkdir -p $(BK_DIR)
	mkdir -p $(BD_DIR)
	mkdir -p $(BA_DIR)
	mkdir -p $(BCPU_DIR)
	dd if=/dev/zero of=$(ZERO_FILE) bs=1MB count=1



$(BK_DIR)/kernel.o: $(KERNEL_DIR)/kernel.c
	gcc $(CFLAGS) -ffreestanding -fno-pie -c $^ -o $@

$(BK_DIR)/test.o: $(KERNEL_DIR)/test.c
	gcc  $(CFLAGS) -ffreestanding -fno-pie -c $^ -o $@

$(BD_DIR)/ports.o: $(DRIVER_DIR)/ports.c
	gcc  $(CFLAGS) -ffreestanding -fno-pie -c $^ -o $@

$(BD_DIR)/keyboard.o: $(DRIVER_DIR)/keyboard.c
	gcc  $(CFLAGS) -ffreestanding -fno-pie -c $^ -o $@


# --- 
$(BCPU_DIR)/timer.o: $(CPU_DIR)/timer.c
	gcc  $(CFLAGS) -ffreestanding -fno-pie -c $^ -o $@

$(BCPU_DIR)/idt.o: $(CPU_DIR)/idt.c
	gcc  $(CFLAGS) -ffreestanding -fno-pie -c $^ -o $@

$(BCPU_DIR)/isr.o : $(CPU_DIR)/isr.c
	gcc  $(CFLAGS) -ffreestanding -fno-pie -c $^ -o $@

$(BCPU_DIR)/interrupt.o: $(CPU_DIR)/interrupt.asm
	nasm $^ -f elf32 -o $@ 	

# ------- asm inc  bin

incs: $(BUILD_DIR)/kernel/inc.bin


$(BUILD_DIR)/kernel/inc.bin: $(KERNEL_DIR)/inc.asm
	nasm $^ -f bin -o $@

# -------asm
$(BUILD_DIR)/bootsect.bin: $(ASM_DIR)/boot_sect_main.asm
	nasm $^ -f bin -o $@

$(BUILD_DIR)/kernel_entry.o: $(ASM_DIR)/kernel_entry.asm
	nasm $^ -f elf32 -o $@ 

$(BUILD_DIR)/kernel/32bit-mem.o: $(KERNEL_DIR)/32bit-mem.asm
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



BUILD_DIR = build
TEMP_DIR = temp
KERNEL_DIR = kernel
DRIVER_DIR = drivers
ASM_DIR = asm

BK_DIR = build/kernel
BD_DIR = build/drivers
CFLAGS = -g -std=c23 -m32
LD_FLAGS = -m elf_i386


objects= $(BUILD_DIR)/kernel_entry.o $(BK_DIR)/kernel.o $(BK_DIR)/test.o $(BD_DIR)/ports.o


.SILENT:make_objects

#---------------


 

all: echos init build_all
	
	
build_all: $(BUILD_DIR)/bootsect.bin $(BUILD_DIR)/kernel.bin
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



$(BK_DIR)/kernel.o: $(KERNEL_DIR)/kernel.c
	gcc $(CFLAGS) -ffreestanding -fno-pie -c $^ -o $@

$(BK_DIR)/test.o: $(KERNEL_DIR)/test.c
	gcc  $(CFLAGS) -ffreestanding -fno-pie -c $^ -o $@

$(BD_DIR)/ports.o: $(DRIVER_DIR)/ports.c
	gcc  $(CFLAGS) -ffreestanding -fno-pie -c $^ -o $@


$(BUILD_DIR)/bootsect.bin: $(ASM_DIR)/boot_sect_main.asm
	nasm $^ -f bin -o $@

$(BUILD_DIR)/kernel_entry.o: $(ASM_DIR)/kernel_entry.asm
	nasm $^ -f elf32 -o $@ 





$(BUILD_DIR)/kernel.bin: $(objects)
	ld  -Ttext 0x1000  $(LD_FLAGS) --oformat binary -Map $(BUILD_DIR)/kernel_entry.map -o $@ $^ 

$(BUILD_DIR)/kernel.elf: $(objects)
	ld -Ttext 0x1000  $(LD_FLAGS) -o $@ $^  


elf: $(BUILD_DIR)/kernel.elf
	@echo "Make elf"

debug: $(BUILD_DIR)/os-image.bin $(BUILD_DIR)/kernel.elf
	gdb	-ex "target remote 172.26.112.1:1234" -ex "symbol-file $(BUILD_DIR)/kernel.elf"

rebuild:init clean 
	@make

# ---------------------------------------------------
# kernel_:
# 	@echo "[= Target kernel =]" 
# 	rm -f bootsect.bin kernel.bin os-image.bin disk.img

# 	gcc -m32 -ffreestanding -fno-pie -c ./kernel/kernel.c -o ./tmp/kernel.o
# 	nasm ./asm/kernel_entry.asm -f elf32 -o ./tmp/kernel_entry.o


# 	ld  -Ttext 0x1000  -m elf_i386 --oformat binary -Map map.map -o kernel.bin ./tmp/kernel_entry.o ./tmp/kernel.o

# 	ld  -Ttext 0x1000  -m elf_i386 -Map map.map -o kernel.elf ./tmp/kernel_entry.o ./tmp/kernel.o

# 	objcopy -O binary kernel.elf kernel.elf.bin
# 	nasm ./asm/boot_sect_main.asm -f bin -o bootsect.bin
# 	cat bootsect.bin  kernel.bin > os-image.bin
# 	cat bootsect.bin  kernel.elf.bin > os-image.elf.bin



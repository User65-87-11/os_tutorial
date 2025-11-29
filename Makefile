BUILD_DIR = build
KERNEL_DIR = kernel
DRIVER_DIR = drivers
ASM_DIR = asm

BK_DIR = build/kernel
BD_DIR = build/drivers
CFLAGS = -g
.SILENT:make_objects

#---------------


all: echos init build_all
	
	
build_all: $(BUILD_DIR)/bootsect.bin $(BUILD_DIR)/kernel.bin
	cat $^ > $(BUILD_DIR)/os-image.bin

echos:
	@echo "-----------BUILD------------"
	@echo $(BK_DIR)
clean:
	@echo "-----------CLEAN------------"
	rm -rf $(BUILD_DIR)/*

init:
	@echo "-----------INIT------------"
	mkdir -p $(BK_DIR)
	mkdir -p $(BK_DIR)


$(BK_DIR)/kernel.o: $(KERNEL_DIR)/kernel.c
	gcc -m32 $(CFLAGS) -ffreestanding -fno-pie -c $^ -o $@

$(BK_DIR)/test.o: $(KERNEL_DIR)/test.c
	gcc -m32 $(CFLAGS) -ffreestanding -fno-pie -c $^ -o $@


$(BUILD_DIR)/kernel_entry.o: $(ASM_DIR)/kernel_entry.asm
	nasm $^ -f elf32 -o $@ 

$(BUILD_DIR)/kernel.bin: $(BUILD_DIR)/kernel_entry.o $(BK_DIR)/kernel.o $(BK_DIR)/test.o
	ld  -Ttext 0x1000  -m elf_i386 --oformat binary -Map $(BUILD_DIR)/kernel_entry.map -o $@ $^

$(BUILD_DIR)/kernel.elf: $(BUILD_DIR)/kernel_entry.o $(BK_DIR)/kernel.o $(BK_DIR)/test.o
	ld  -Ttext 0x1000  -m elf_i386  -o $@ $^

make_elf: $(BUILD_DIR)/kernel.elf
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



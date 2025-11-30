[org 0x7c00] ; bootloader offset
	KERNEL_OFFSET equ 0x1000 
_start:
    mov [BOOT_DRIVE], dl ; Remember that the BIOS sets us the boot drive in 'dl' on boot

	call print_hex



    mov bp, 0x9000
    mov sp, bp

    mov bx, MSG_REAL_MODE 
    call print
    call print_nl

    call load_kernel ; read the kernel from disk
    call switch_to_pm ; disable interrupts, load GDT,  etc. Finally jumps to 'BEGIN_PM'
    jmp $ ; Never executed



%include "./asm/boot_sect_print.asm"
%include "./asm/boot_sect_print_hex.asm"
%include "./asm/boot_sect_disk.asm"
%include "./asm/32bit-gdt.asm"
%include "./asm/32bit-print.asm"
%include "./asm/32bit-switch.asm"



[bits 16]
load_kernel:
	mov bx, MSG_LOAD_KERNEL
    call print
    call print_nl

    mov bx, KERNEL_OFFSET ; Read from disk and store at 0x1000
    mov dh, 54 ; 54 ; 6 bits ; 8 bits for heads; 10 bits fo cyls
    mov dl, [BOOT_DRIVE]
 
    call disk_load

    ret


[bits 32]
BEGIN_PM: ; after the switch we will get here
	mov ebx, MSG_PROT_MODE
    call print_string_pm
    call KERNEL_OFFSET ; Give control to the kernel
    jmp $ ; Stay here when the kernel returns control to us (if ever)

BOOT_DRIVE db 0x00 ; It is a good idea to store it in memory because 'd C=0x80 a=0x00;
MSG_REAL_MODE db "Started in 16-bit Real Mode", 0
MSG_PROT_MODE db "Landed in 32-bit Protected Mode", 0
MSG_LOAD_KERNEL db "Loading kernel into memory", 0

times 510 - ($-$$) db 0
dw 0xaa55 


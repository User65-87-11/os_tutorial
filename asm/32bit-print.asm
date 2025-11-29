[bits 32] ; using 32-bit protected mode

; this is how constants are defined
VIDEO_MEMORY equ 0xb8000
SYM_COL equ 0x07 ; the color byte for each character

print_string_pm:
    pusha
    mov edx, VIDEO_MEMORY

print_string_pm_loop:
    mov al, [ebx] ; [ebx] is the address of our character
    mov ah, SYM_COL

    cmp al, 0 ; check if end of string
    je print_string_pm_done

    mov [edx], ax ; store character + attribute in video memory
    add ebx, 1 ; next char
    add edx, 2 ; next video memory position

    jmp print_string_pm_loop

print_string_pm_done:
    popa
    ret

clear_scr:
	pusha
    mov edx, VIDEO_MEMORY
	mov cx,80*25
	mov al,0
	mov ah,0
	L1:
		mov [edx],ax
		add edx,2
		DEC cx
		JNZ L1
	popa
	ret
global mem_set

global mem_copy

global mem_rev_copy

global str_len
global mem_compare

; stack is later in 64bit 
; gcc
; order f(rdi,rsi,rdx,rcx,r8,r9, stack...)
; rdi, rsi, rdx, rcx, r8, r9  = gcc
 

mem_copy:
	mov rcx, rdx
	cld
	rep movsb
	ret

mem_compare:
	; dst,src,src len ; 1 = true ; 0 =not true
	mov eax, 1
	mov rcx,rdx
	cld
	repe cmpsb 
	jz mem_compare_exit
	mov rax,0
mem_compare_exit:
	ret 




str_len:
	mov rsi,rdi
	xor al, al
	repne scasb
	sub rdi,rsi
	dec rdi
	mov rax,rdi
	ret


mem_rev_copy:
		cmp rdx,0
		je mem_rev_copy_exit
		add rsi , rdx
		dec rsi 
	mem_rev_copy_loop:
        mov cl, [rsi]
        mov [rdi], cl
        dec rsi
        inc rdi
		dec rdx
        cmp rdx, 0
        jg mem_rev_copy_loop
	mem_rev_copy_exit:
        ret	

mem_set:
	mov rcx, rdx
	mov al, [rsi]  
	cld
	rep stosb
	ret

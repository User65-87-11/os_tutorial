global mem_set32

global mem_copy32

global mem_rev_copy32

global str_len32
global mem_compare32

; stack is later in 64bit 
; gcc
; order f(edi,esi,edx,ecx,r8,r9, stack...)
; edi, esi, edx, ecx, r8, r9  = gcc
 

mem_copy32:
		mov ecx, edx
		cld
		rep movsb
		ret

mem_compare32:
	; dst,src,src len ; 1 = true ; 0 =not true
		mov eax, 1
		mov ecx,edx
		cld
		repe cmpsb 
	jz mem_compare_exit
		mov eax,0
	mem_compare_exit:
		ret 




str_len32:
		mov esi,edi
		xor al, al
		repne scasb
		sub edi,esi
		dec edi
		mov eax,edi
		ret


mem_rev_copy32:
		cmp edx,0
		je mem_rev_copy_exit
		add esi , edx
		dec esi 
	mem_rev_copy_loop:
        mov cl, [esi]
        mov [edi], cl
        dec esi
        inc edi
		dec edx
        cmp edx, 0
        jg mem_rev_copy_loop
	mem_rev_copy_exit:
        ret	

mem_set32:
		mov ecx, edx
		mov al, [esi]  
		cld
		rep stosb
		ret

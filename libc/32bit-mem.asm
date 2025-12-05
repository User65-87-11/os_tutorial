global mem_set32

global mem_copy32

global str_reverse32

global str_len32

mem_copy32:
	push ebp
	mov ebp,esp
	mov ecx, DWORD   [ebp + 16]
	mov esi, DWORD   [ebp + 12]
	mov edi, DWORD   [ebp + 8]
	cld
	rep movsb
	leave
	ret


global str_len32:
	mov edi, DWORD   [ebp]
	mov esi,edi
	xor al, al
	repne scasb
	sub edi, esi
	mov eax, edi
	ret


str_reverse32:
		mov ecx, DWORD   [ebp + 12]
		cmp ecx,0
		jmp str_reverse32_exit

		mov esi, DWORD   [ebp + 8]
		mov edi, DWORD   [ebp ]
		add esi , ecx
		dec esi 
		

str_reverse32_loop:
	
        mov dl, [esi]
        mov [edi], dl
        dec esi
        inc edi
		dec ecx
        cmp ecx, 0
        jg reverse32_loop
str_reverse32_exit:
        ret	

mem_set32:
	push ebp
	mov ebp,esp
	mov ecx, DWORD   [ebp + 16]
	mov al, BYTE   [ebp + 12]
	mov edi, DWORD   [ebp + 8]
	cld
	rep stosb
	leave
	ret

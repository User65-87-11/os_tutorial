global rep_stosb

global rep_movsb


rep_movsb:
	push ebp
	mov ebp,esp
	mov ecx, DWORD   [ebp + 16]
	mov esi, DWORD   [ebp + 12]
	mov edi, DWORD   [ebp + 8]
	rep movsb
	leave
	;mov ecx,edx
	rep movsb
	ret


rep_stosb:
	push ebp
	mov ebp,esp
	mov ecx, DWORD   [ebp + 16]
	mov al, BYTE   [ebp + 12]
	mov edi, DWORD   [ebp + 8]
	; mov ecx,edx
	rep stosb
	leave
	ret

	; pop ecx (len)
	; pop esi (src)
	; pop edi (dst)
	; rep movsb

; 0000192d <port_word_in>:
;     192d:       55                      push   %ebp
;     192e:       89 e5                   mov    %esp,%ebp
;     1930:       83 ec 14                sub    $0x14,%esp
;     1933:       8b 45 08                mov    0x8(%ebp),%eax
;     1936:       66 89 45 ec             mov    %ax,-0x14(%ebp)
;     193a:       0f b7 45 ec             movzwl -0x14(%ebp),%eax
;     193e:       89 c2                   mov    %eax,%edx
;     1940:       66 ed                   in     (%dx),%ax
;     1942:       66 89 45 fe             mov    %ax,-0x2(%ebp)
;     1946:       0f b7 45 fe             movzwl -0x2(%ebp),%eax
;     194a:       c9                      leave
;     194b:       c3                      ret

	
;extern void rep_movsb(char *dst,char *src,int num);

; The address of the source operand is read from the DS:ESI or the DS:SI registers (depending on the address-size attribute of the instruction, 32 or 16, respectively). The address of the destination operand is read from the ES:EDI or the ES:DI registers (again depending on the address-size attribute of the instruction). The DS segment may be over-ridden with a segment override prefix, but the ES segment cannot be overridden.

; ; line 42:    
; 	push 3 (len)
; 	push    2(src)
; 	push    1(dst)
; 	call    ?cdeclAdd@@YAHHH@Z            ; cdeclAdd
; 	add    esp, 8
; 	mov    DWORD PTR _result$[ebp], eax
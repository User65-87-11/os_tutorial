BITS 32
ORG 0

global inc_fill_struct


inc_fill_struct:

	
	mov [esp + 0],eax
	mov [esp + 4],ebx
	mov [esp + 8],ecx
	mov [esp + 12],edx
	mov [esp + 16],esi
	mov [esp + 20],edi

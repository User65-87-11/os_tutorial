#pragma once
#include "../drivers/screen.h"
//#include "../kernel/bytes.h"
#include "../libc/strings.h"

volatile void print_regs();

typedef struct Regs Regs;

extern void asm_print_regs32(Regs * r);

#ifdef PRINT_REGS_IMPLEMENTATION

typedef struct Regs{
	unsigned int eax;
	unsigned int ebx;
	unsigned int ecx;
	unsigned int edx;
	unsigned int esi;
	unsigned int edi;


}Regs;

volatile void print_regs(){
	//ice hacky
	Regs r={};
	
	__asm__ volatile 	(                                          
		"lea %0,%%esp\n"
		"add $20,%%esp\n"
		"push %%esi\n"
		"push %%edx\n"
		"push %%ecx\n"
		"push %%ebx\n"
		"push %%eax\n"
		"push %%edi\n"
		: 
		: "m"(r)
		: "memory"
	);


//__asm__(".incbin \"build/kernel/inc.bin\"");




	print_cstring("     eax,     ebx,     ecx,     edx,     esi,     edi\n");
	char string[9] ={};

	for(int i=0;i<6;i++)
	{
		int * base = (int *)&r;
		((int *)string)[0] = 0;
		((int *)string)[1] = 0;
		
		itoa32(base[i],string,16);
		
		int len = str_len32(string);
		for(int a=0;a<8-len - 1;a++)
		{
			print_cstring("0");
		}
		print_cstring(string);
		print_cstring(" ");
	}
 	print_cstring("\n");

}
#endif
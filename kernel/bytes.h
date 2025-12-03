#pragma once
#include "types.h"


int cstr_len(char * c);
void itoa(int val,char * buff, int radix);//returns filled buffer and the new length
void memset(char * c,int len, char val);

extern  void rep_movsb(char *dst,char *src,int num);
extern  void rep_stosb(char *dst,char byte,int num);

void rep_movsb$loc(void *dst,void *src,int num);


#ifdef KERNEL_BYTES_IMPLEMENTATION

void rep_movsb$loc(void *dst,void *src,int num){
	__asm__ 	(                                          
	"cld\n\t"                                
	"rep\n\t"                                
	"movsb"                                  
	:                                        
	: "S" (src), "D" (dst), "c" (num)  
	:   "memory"
	);

	// __asm__("cld ; rep movsb"
	// 	 :
	// 	 : "S"(src), "D"(dst), "c"(num) 
	// 	 : "flags", "memory");

		// asm(".intel_syntax noprefix");
		// asm("mov eax, ebx");
}

void memset(char * c,int len, char val){
	for(int i=0;i<len;i++)
	{
		c[i] = val;
	}
}


/* 
buffer the size of #define ITOA_BUFF_SIZE 14
	copypasted it from the github
	
	converts @value to a string of printable chars 
		copies them to @buffer
	@radix is 10,16 or something >1

*/
void itoa(int value,char * buff,int radix){
	
	int val;
    int negative;
    char buffer[14];
    char *pos;
    int digit;
 
    if (value < 0 && radix == 10) {
    	negative = 1;
        val = -value;
    } else {
    	negative = 0;
        val = value;
    } /* if */
 
    pos = &buffer[14];
    *pos = '\0';
 
    do {
   	 	digit = val % radix;
    	val = val / radix;
    if (digit < 10) {
        *--pos = '0' + digit;
    } else {
        *--pos = 'a' + digit - 10;
    } /* if */
    } while (val != 0L);
 
    if (negative) {
    *--pos = '-';
    } /* if */

	int num  = &buffer[14] - pos ;
	for(int i=0;i< num; i++){
		buff[i] = pos[i];
	}
 //   memcpy(string, pos, &buffer[64] - pos + 1);
   // return string;
	 
}

int cstr_len(char *c){
	int l = 0;
	char *t = c;
	while(*++t != '\0');
	l  = t - c -1;
	return l;
}
#endif
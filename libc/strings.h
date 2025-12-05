#pragma once
// #include "types.h"


int str_len(char * c);
void itoa32(int val,char * buff, int radix);//returns filled buffer and the new length
// void memset(char * c,int len, char val);

extern  void mem_copy32(char *dst,char *src,int num);
extern  void mem_set32(char *dst,char byte,int num);
extern  void str_reverse32(char *dst,char byte,int num);
extern  int str_len32(char *str);


void rep_movsb32$loc(void *dst,void *src,int num);


#ifdef STRINGS_IMPLEMENTATION

void rep_movsb32$loc(void *dst,void *src,int num){
	__asm__ 	(                                          
	"cld\n\t"                                
	"rep\n\t"                                
	"movsb"                                  
	:                                        
	: "S" (src), "D" (dst), "c" (num)  
	:   "memory"
	);

	 
}

int search_array(char * str,int lensearch,char*bytes, int lenkey){
	char *a;
	char *b;
	bool found = false;
	int j = -1;
	if(lensearch < lenkey) {

		return j;
	}
	
	for(j = 0; j < lensearch; j++)
	{
		if(str[j] == *bytes){
			found = true;
			for(int i=1; i < lenkey; i++){
				a = str+j+i;
				b = bytes + i;
				if(*a != *b){
					found= false;
					break;
				}
			}
			if(found){
				break;
			}
		}
	}
	if(found == false)
	{
		j = -1;
	}
	return j;
}
int search_mem(char * pattern, int p_len ,int start, int len){
	bool found = false ;
	char * t = pattern;
	int pos=-1;

	for(char *i = (char*)start; (int)i < (start + len); i++)
	{
		if(*i == *t){
			pos = search_array(i,p_len,t,p_len);
			if(pos != -1){
				pos+=(int)i;
				break;
			}
		}
	}
	return pos;
}

 
void itoa32(int value,char * buff,int radix){
	#define ITOA_BUFF_SIZE 14

	int val;
    char negative;
    char buffer[ITOA_BUFF_SIZE];
    char *pos;
    int digit;
 
    if (value < 0 && radix == 10) {
    	negative = 1;
        val = -value;
    } else {
    	negative = 0;
        val = value;
    } /* if */
 
    pos = &buffer[ITOA_BUFF_SIZE];
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

 
int str_len(char *c){
	int l = 0;
	while(c[l++] != '\0');
	return l;
}
#endif
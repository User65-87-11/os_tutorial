#include "../drivers/ports.h"

#define DRIVERS_SCREEN_IMPLEMENTATION
#include "../drivers/screen.h"

#define KERNEL_BYTES_IMPLEMENTATION
#include "bytes.h"

#include "types.h"

typedef unsigned char uint8_t ;
typedef unsigned short uint16_t ;
#define VGA_WIDTH  80
#define VGA_HEIGHT  25
#define ITOA_BUFF_SIZE 14


// void set_cur_pos(int pos);
// void set_cur_xy(int x,int y);
// void print_cstring(char *c);
// void print_cstring_at(char *s,int x,int y);
// void scroll();
// void put_char(char c,int pos);


// int cstr_len(char * c);
// void itoa(int val,char * buff, int radix);//returns filled buffer and the new length
// void zero_buff(char * c,int len);


// void zero_buff(char * c,int len){
// 	for(int i=0;i<len;i++)
// 	{
// 		c[i] = 0;
// 	}
// }
// void itoa(int value,char * buff,int radix){
	
// 	int val;
//     int negative;
//     char buffer[ITOA_BUFF_SIZE];
//     char *pos;
//     int digit;
 
//     if (value < 0 && radix == 10) {
//     	negative = 1;
//         val = -value;
//     } else {
//     	negative = 0;
//         val = value;
//     } /* if */
 
//     pos = &buffer[ITOA_BUFF_SIZE];
//     *pos = '\0';
 
//     do {
//    	 	digit = val % radix;
//     	val = val / radix;
//     if (digit < 10) {
//         *--pos = '0' + digit;
//     } else {
//         *--pos = 'a' + digit - 10;
//     } /* if */
//     } while (val != 0L);
 
//     if (negative) {
//     *--pos = '-';
//     } /* if */

// 	int num  = &buffer[ITOA_BUFF_SIZE] - pos ;
// 	for(int i=0;i< num; i++){
// 		buff[i] = pos[i];
// 	}
//  //   memcpy(string, pos, &buffer[64] - pos + 1);
//    // return string;
	 
// }

// int cstr_len(char *c){
// 	int l = 0;
// 	char *t = c;
// 	while(*++t != '\0');
// 	l  = t - c -1;
// 	return l;
// }
// uint16_t get_cur_pos(){

// 	port_byte_out(0x3d4, 14); /* Requesting byte 14: high byte of cursor pos */
//     /* Data is returned in VGA data register (0x3d5) */
//     uint16_t position = port_byte_in(0x3d5);
//     position = position << 8; /* high byte */

//     port_byte_out(0x3d4, 15); /* requesting low byte */
//     position += port_byte_in(0x3d5);

// 	return position;
// }
// void print_memory(int addr, int len){
// 	char * a = (char *)addr;

// 	for(int i=0;i<len;i++){
// 		 char cl = a[i] & 0x0f;
// 		 char ch = (a[i] >> 4) & 0x0f;
		
// 		cl+=0x30;
// 		ch+=0x30;
// 		if(cl > '9')
// 			cl+= 7;
// 		if(ch > '9')
// 			ch+= 7;
		
// 		put_char(cl,i*2);
// 		put_char(ch,i*2 +1);
		
// 	}
// 	print_cstring("\n");
// }
// void put_char(char c, int pos ){
// 	char *vga = (char*)0xb8000;
// 	vga[pos*2] = c;
// 	vga[pos*2 + 1] = 0x0e;
// }
// void print_cstring_at(char *s,int x,int y){
// 	int pos = y*VGA_WIDTH + x;
// 	while(*s !='\0'){
// 		put_char(*s++,pos++);
// 	}
// }
// void print_cstring(char * s){
// 	uint16_t cur = get_cur_pos();
	
// 	while(*s!='\0')
// 	{
// 		if(cur < VGA_WIDTH * VGA_HEIGHT)
// 		{

			
// 			if(*s == '\n'){
				
// 				cur = cur + (VGA_WIDTH - (cur % VGA_WIDTH))  - 1;
// 			}
// 			else{
// 				put_char(*s,cur);
// 			}

			
// 			cur ++;
// 			s++;	
// 		}
// 	}
// 	set_cur_pos(cur);
// }

// void clear_scr(){
// 	char *vga = (char*)0xb8000;


// 	for(int i=0;i< VGA_WIDTH * VGA_HEIGHT * 2 ;i+=2){
// 		vga[i] = '\0';
// 		vga[i+1] = 0x0f;
		
// 	}
// }

// void set_cur_pos(int pos){
// 	//uint16_t pos = y * VGA_WIDTH + x;

// 	port_byte_out(0x3D4, 0x0F);
// 	port_byte_out(0x3D5, (uint8_t) (pos & 0xFF));
// 	port_byte_out(0x3D4, 0x0E);
// 	port_byte_out(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
// }
// void set_cur_xy(int x,int y){
// 	uint16_t pos = y * VGA_WIDTH + x;

// 	port_byte_out(0x3D4, 0x0F); // low
// 	port_byte_out(0x3D5, (uint8_t) (pos & 0xFF));
// 	port_byte_out(0x3D4, 0x0E); //high
// 	port_byte_out(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
// }

//fix me
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
void main() {
	clear_scr();
	set_cur_pos(0);
    /* Screen cursor position: ask VGA control register (0x3d4) for bytes
     * 14 = high byte of cursor and 15 = low byte of cursor. */
    port_byte_out(0x3d4, 14); /* Requesting byte 14: high byte of cursor pos */
    /* Data is returned in VGA data register (0x3d5) */
    int position = port_byte_in(0x3d5);
    position = position << 8; /* high byte */

    port_byte_out(0x3d4, 15); /* requesting low byte */
    position += port_byte_in(0x3d5);

    /* VGA 'cells' consist of the character and its control data
     * e.g. 'white on black background', 'red text on white bg', etc */
    int offset_from_vga = position * 2;

    /* Now you can examine both variables using gdb, since we still
     * don't know how to print strings on screen. Run 'make debug' and
     * on the gdb console:
     * breakpoint kernel.c:21
     * continue
     * print position
     * print offset_from_vga
     */

    /* Let's write on the current cursor position, we already know how
     * to do that */
    char *vga = (char*)0xb8000;
    vga[offset_from_vga] = 'X'; 
    vga[offset_from_vga+1] = 0x0f; /* White text on black background */

	set_cur_xy(0,0);
	char hello_str[]="Hello world!!!!!\n";
	int hello_str_len = cstr_len(hello_str);
	char search_pat[] ="world";
	int search_pat_len = cstr_len(search_pat);
	

	char hello_str1[]="12345";
	char new_line[]="\n";
	int hello_str1_len = cstr_len(hello_str);

	char buff[ITOA_BUFF_SIZE]={0};



	print_cstring(hello_str);
	print_cstring(hello_str);
	print_cstring(hello_str);

	int find = search_array(
		hello_str,
		hello_str_len,
		search_pat,
		search_pat_len);

	itoa(find,buff,10);
	print_cstring(buff);
	print_cstring(new_line);
	
	char empty []= "Empty!";

	char * c = (char*)0x10000;
	c[0] = 'T';
	c[1] = 'e';
	c[2] = 's';
	c[3] = 't';
	c[4] = '\0';
	
	char test[]="findme"; //finds this
	int mem_pos = search_mem(test,5, 0x0, 0x100000);
	

	
	// print_cstring(new_line);
	itoa(mem_pos,buff,16);

	
	print_cstring(buff);
	print_cstring(new_line);
	print_cstring(test);
	print_cstring(new_line);
	print_cstring(test);
	if(mem_pos >= 0)
	{
	
		//((char*)mem_pos)[0]='E';
	
		print_cstring(new_line);
		print_cstring((char*)mem_pos);
	}
	else{
		print_cstring(empty);
	}

	print_cstring(new_line);
	print_cstring("findme");
	print_cstring(new_line);

	print_memory(0x8ff82,64);

	// print_cstring(new_line);
	// itoa(mem_pos,buff,16);

	// print_cstring(buff);

	// print_cstring("testa");
	
	// print_cstring("testc");
	//print_cstring(test);
	
	// print_cstring_at(buff,0,0);

	// zero_buff(buff,ITOA_BUFF_SIZE);

	// len2 =itoa(len2,buff,16);
 
	// print_cstring_at(buff,0,1);

}
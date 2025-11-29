#include "../drivers/ports.h"

typedef unsigned char uint8_t ;
typedef unsigned short uint16_t ;
#define VGA_WIDTH  80
#define VGA_HEIGHT  25

int str_len(char * c);
void set_cur_pos(int pos);
void set_cur_xy(int x,int y);
void print_string(char *c, int len);
int itoa(int val,char * buff, int radix);//returns filled buffer and the new length


char *test_c = "Test!";

int itoa(int value,char * buff,int radix){
	
	int val;
    int negative;
    char buffer[64];
    char *pos;
    int digit;
 
    if (value < 0 && radix == 10) {
    	negative = 1;
        val = -value;
    } else {
    	negative = 0;
        val = value;
    } /* if */
 
    pos = &buffer[64];
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

	int num  = &buffer[64] - pos ;
	for(int i=0;i< num; i++){
		buff[i] = pos[i];
	}
 //   memcpy(string, pos, &buffer[64] - pos + 1);
   // return string;
	return num;
}

int str_len(char *c){
	int l = 0;
	char *t = c;
	while(*++t != '\0');
	l  = t - c -1;
	return l;
}
uint16_t get_cur_pos(){

	port_byte_out(0x3d4, 14); /* Requesting byte 14: high byte of cursor pos */
    /* Data is returned in VGA data register (0x3d5) */
    uint16_t position = port_byte_in(0x3d5);
    position = position << 8; /* high byte */

    port_byte_out(0x3d4, 15); /* requesting low byte */
    position += port_byte_in(0x3d5);

	return position;
}
void put_char(char c, uint16_t pos ){
	char *vga = (char*)0xb8000;
	vga[pos*2] = c;
	vga[pos*2 + 1] = 0x0e;
}
void print_string(char * s,int len){
	uint16_t cur = get_cur_pos();
	
	for(int i=0; i<len;i++)
	{
		if(cur < VGA_WIDTH * VGA_HEIGHT)
		{
			put_char(s[i],cur);
			cur ++;
			set_cur_pos(cur);
			
			// cur = get_cur_pos();
			
		}
	}
}

void clear_scr(){
	char *vga = (char*)0xb8000;


	for(int i=0;i< VGA_WIDTH * VGA_HEIGHT * 2 ;i+=2){
		vga[i] = '\0';
		vga[i+1] = 0x0f;
		
	}
}

void set_cur_pos(int pos){
	//uint16_t pos = y * VGA_WIDTH + x;

	port_byte_out(0x3D4, 0x0F);
	port_byte_out(0x3D5, (uint8_t) (pos & 0xFF));
	port_byte_out(0x3D4, 0x0E);
	port_byte_out(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}
void set_cur_xy(int x,int y){
	uint16_t pos = y * VGA_WIDTH + x;

	port_byte_out(0x3D4, 0x0F);
	port_byte_out(0x3D5, (uint8_t) (pos & 0xFF));
	port_byte_out(0x3D4, 0x0E);
	port_byte_out(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}
//fix me
int search_mem(char * pattern , int start, int len){
	char found = 0 ;
	char * t = pattern;
	int pattern_len = str_len(pattern);
	int pos=0;
	int temp_pen = 0;
	for(char *i = (char*)start; (int)i < (start + len); i++)
	{
		if(*i == *t){
			pos= (int)i;
			found = 1;
			temp_pen=1;
			while(*++t!='\0'){
				if(*t != *++i)
				{
					t = pattern;
					found = 0;
					i = (char *)pos;
					break;
				}
				temp_pen++;
			}
			if(temp_pen !=pattern_len) {
				t = pattern;
				found = 0;
				i = (char *)pos;
				
			}
			if(found){
				break;
			}
		}
	}
	return pos;
}
void main() {
	clear_scr();
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
	char hello_str[]="Hello world!!!!!";
	int len = str_len(hello_str);

	char buff[10]={0};


	print_string(hello_str,len);

	char   search_str[6]={0};
	search_str[0] = 'T';
	search_str[1] = 'e';
	search_str[2] = 's';
	search_str[3] = 't';
	search_str[4] = '!';
	int mem_pos = search_mem(search_str,0,0x10000);

	int len2 =itoa(mem_pos,buff,16);

	print_string(hello_str,len);

	print_string(buff,len2);
	
	print_string((char *)mem_pos,str_len((char *)mem_pos));

 
 

}
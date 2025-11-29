#include "../drivers/ports.h"

typedef unsigned char uint8_t ;
typedef unsigned short uint16_t ;
#define VGA_WIDTH  80
#define VGA_HEIGHT  25

int str_len(char * c);
void set_cur_pos(int pos);
void set_cur_xy(int x,int y);
void print_string(char *c, int len);


int str_len(char *c){
	int l =0;
	while(c[++l]!='\0');
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
	char hello_str[]="printf(\"Hello world\");";
	int len = str_len(hello_str);

	print_string(hello_str,len);
	print_string(hello_str,len);
	 
}
#pragma once
#include "../kernel/types.h"

#define VGA_WIDTH  80
#define VGA_HEIGHT  25

void set_cur_xy(int x,int y);
uint16_t get_cur_pos();;
void print_memory(int addr, int len);
void put_char(char c, int pos );
void print_cstring_at(char *s,int x,int y);
void print_cstring(char * s);
void clear_scr();
void set_cur_pos(int pos);
void set_cur_xy(int x,int y);



#ifdef DRIVERS_SCREEN_IMPLEMENTATION




uint16_t get_cur_pos(){

	port_byte_out(0x3d4, 14); /* Requesting byte 14: high byte of cursor pos */
    /* Data is returned in VGA data register (0x3d5) */
    uint16_t position = port_byte_in(0x3d5);
    position = position << 8; /* high byte */

    port_byte_out(0x3d4, 15); /* requesting low byte */
    position += port_byte_in(0x3d5);

	return position;
}
void print_memory(int addr, int len){
	char * a = (char *)addr;

	for(int i=0;i<len;i++){
		 char cl = a[i] & 0x0f;
		 char ch = (a[i] >> 4) & 0x0f;
		
		cl+=0x30;
		ch+=0x30;
		if(cl > '9')
			cl+= 7;
		if(ch > '9')
			ch+= 7;
		
		put_char(cl,i*2);
		put_char(ch,i*2 +1);
		
	}
	print_cstring("\n");
}
void put_char(char c, int pos ){
	char *vga = (char*)0xb8000;
	vga[pos*2] = c;
	vga[pos*2 + 1] = 0x0e;
}
void print_cstring_at(char *s,int x,int y){
	int pos = y*VGA_WIDTH + x;
	while(*s !='\0'){
		put_char(*s++,pos++);
	}
}
void print_cstring(char * s){
	uint16_t cur = get_cur_pos();
	
	while(*s!='\0')
	{
		if(cur < VGA_WIDTH * VGA_HEIGHT)
		{

			
			if(*s == '\n'){
				
				cur = cur + (VGA_WIDTH - (cur % VGA_WIDTH))  - 1;
			}
			else{
				put_char(*s,cur);
			}

			
			cur ++;
			s++;	
		}
	}
	set_cur_pos(cur);
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

	port_byte_out(0x3D4, 0x0F); // low
	port_byte_out(0x3D5, (uint8_t) (pos & 0xFF));
	port_byte_out(0x3D4, 0x0E); //high
	port_byte_out(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}
#endif
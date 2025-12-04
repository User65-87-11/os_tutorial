#pragma once

#include "../kernel/bytes.h"
#include "ports.h"



#define VGA_WIDTH  80
#define VGA_HEIGHT  25





void set_cur_xy(int x,int y);
unsigned short get_cur_pos();;
void print_memory(int addr, int len);
void put_char(char c, int pos );
void print_cstring_at(char *s,int x,int y);
void print_cstring(char * s);
void clear_scr();
void set_cur_pos(int pos);
void set_cur_xy(int x,int y);
void scroll_by(int y);
void scroll_by_v2(int y);
void scroll_by_v3( int y);



#ifdef DRIVERS_SCREEN_IMPLEMENTATION

char *VGA_MEM = (char*)0xb8000;

void scroll_by_v3( int y){
	char buffer[VGA_WIDTH*VGA_HEIGHT*2] = {};
	if( y <= 0 ) return;
	if( y > VGA_HEIGHT)
	{
		y = VGA_HEIGHT ;
	}
	rep_movsb$loc(buffer,VGA_MEM,VGA_WIDTH*VGA_HEIGHT * 2);
	int distance = y * VGA_WIDTH;
	int cur_pos =	get_cur_pos();
	int new_cur_pos = cur_pos - distance;
	if(new_cur_pos < 0) 
		new_cur_pos = 0;

	int num = (VGA_WIDTH*VGA_HEIGHT - distance ) * 2;
	rep_movsb$loc(VGA_MEM,&buffer[distance * 2],num);
	
	set_cur_pos(new_cur_pos);


}

void scroll_by_v2(int y){
	char buffer[VGA_WIDTH*VGA_HEIGHT*2] = {};

 

	if( y == 0 ) return;
	if( y > VGA_HEIGHT)
	{
		y = VGA_HEIGHT;
	}else if(y + VGA_HEIGHT <= 0)
	{
		y= -VGA_HEIGHT;
	}
	bool up = true;
	int pos =	get_cur_pos() - y*VGA_WIDTH;
	if(y < 0)
	{
		up = false;
		y = y * -1;
	}
	rep_movsb$loc(buffer,VGA_MEM,VGA_WIDTH*VGA_HEIGHT * 2);
	clear_scr();
	int offset = y * 2 * VGA_WIDTH;
	int num = VGA_WIDTH*VGA_HEIGHT * 2 - offset;
	
	 

	if(up){
		rep_movsb$loc(VGA_MEM,&buffer[offset],num);
		 
	}else
	{
		rep_movsb$loc(&VGA_MEM[offset],buffer,num);
	}

		


	if(pos < 0)
	{
		pos = 0;
	}
	else if( pos >= VGA_WIDTH * VGA_HEIGHT)
	{
			pos = VGA_WIDTH * VGA_HEIGHT - 1;
	}
	set_cur_pos(pos);
}

void scroll_by(int y){
	
	y = y % VGA_HEIGHT;
	if( y == 0 ) return;
	bool up = true;
	if(y < 0)
	{
		up = false;
		y = y * -1;
	}
	unsigned short pos =	get_cur_pos();
 
	int offset = y * VGA_WIDTH * 2;
	

	if(up)
	{		int num = VGA_WIDTH * VGA_HEIGHT * 2 - offset ;
			rep_movsb(VGA_MEM,VGA_MEM + offset,num);//normal
			pos = pos -  (y)*VGA_WIDTH ;
	}else
	{
			int num =  offset ;
			rep_movsb(VGA_MEM + offset ,VGA_MEM ,num);
			pos = pos +  (y)*VGA_WIDTH ;
	}


	
	
	if(pos < 0)
	{
		pos = 0;
	}
	else if( pos >= VGA_WIDTH * VGA_HEIGHT)
	{
			pos = VGA_WIDTH * VGA_HEIGHT - 1;
	}
	set_cur_pos(pos);
}


unsigned short get_cur_pos(){

	port_byte_out(0x3d4, 14); /* Requesting byte 14: high byte of cursor pos */
    /* Data is returned in VGA data register (0x3d5) */
    unsigned short position = port_byte_in(0x3d5);
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
	
	
	while(*s!='\0')
	{
		unsigned short cur = get_cur_pos();
		 
			
			if(*s == '\n'){
				
				cur = cur + (VGA_WIDTH - (cur % VGA_WIDTH))  - 1;
			}
			else{
				put_char(*s,cur);
			}

			
			cur ++;
			s++;	

			set_cur_pos(cur);
			if(cur < (VGA_HEIGHT * VGA_WIDTH))
			{
				
			}else
			{
				scroll_by_v2(1);
				//cur -= VGA_WIDTH;
				//set_cur_pos(cur);
			}
			
			
		 
			

		 
	}
	
}

void clear_scr(){
	char *vga = (char*)0xb8000;


	for(int i=0;i< VGA_WIDTH * VGA_HEIGHT  ;i++){
		vga[i*2] = '\0';
		vga[i*2+1] = 0x0f;
		
	}
}

void set_cur_pos(int pos){
	//uint16_t pos = y * VGA_WIDTH + x;

	port_byte_out(0x3D4, 0x0F);
	port_byte_out(0x3D5, (unsigned char) (pos & 0xFF));
	port_byte_out(0x3D4, 0x0E);
	port_byte_out(0x3D5, (unsigned char) ((pos >> 8) & 0xFF));
}
void set_cur_xy(int x,int y){
	unsigned short pos = y * VGA_WIDTH + x;

	port_byte_out(0x3D4, 0x0F); // low
	port_byte_out(0x3D5, (unsigned char) (pos & 0xFF));
	port_byte_out(0x3D4, 0x0E); //high
	port_byte_out(0x3D5, (unsigned char) ((pos >> 8) & 0xFF));
}
#endif
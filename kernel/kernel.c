#define KERNEL_BYTES_IMPLEMENTATION
#define DRIVERS_SCREEN_IMPLEMENTATION
#define PRINT_REGS_IMPLEMENTATION

#include "../drivers/ports.h"
#include "../drivers/screen.h"
#include "print_regs.h"


#include "bytes.h"


#include "../cpu/isr.h"
#include "../cpu/idt.h"




int main() {
    isr_install();
    /* Test the interrupts */
    __asm__ __volatile__("int $2");
    __asm__ __volatile__("int $3");

	// clear_scr();
	// set_cur_pos(0);
	print_cstring("Hello, World!\n");

	char cur_str[10]= {};
	
	//u16 cur_pos = 0xffff;
	//set_cur_pos(cur_pos);
	// print_cstring("Hello, World!");
	//cur_pos = get_cur_pos();
//	itoa(cur_pos,cur_str,10);

	//print_cstring_at(cur_str,0,0);

	for(int i=0;i<12;i++)
	{
		print_cstring("Hello, World! \n");
	}
	for(int i=0;i<12;i++)
	{
		print_cstring("\n");
	}

	print_regs();
}

#define KERNEL_BYTES_IMPLEMENTATION
#define DRIVERS_SCREEN_IMPLEMENTATION


#include "../drivers/ports.h"


#include "../drivers/screen.h"


#include "bytes.h"


#include "../cpu/isr.h"
#include "../cpu/idt.h"




void main() {
    isr_install();
    /* Test the interrupts */
    __asm__ __volatile__("int $2");
    __asm__ __volatile__("int $3");

	// clear_scr();
	// set_cur_pos(0);
	 print_cstring("Hello, World!");

}

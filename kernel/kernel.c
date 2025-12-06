#define STRINGS_IMPLEMENTATION
#define DRIVERS_SCREEN_IMPLEMENTATION
#define PRINT_REGS_IMPLEMENTATION
#define DETECT_MEMORY_IMPLEMENTATION

#include "../drivers/keyboard.h"
#include "../drivers/screen.h"
#include "../cpu/timer.h"
#include "../libc/strings.h"

#include "detect_mem.h"

//#include "bytes.h"


#include "../cpu/isr.h"
#include "../cpu/idt.h"




void kernel_main() {
    isr_install();
    /* Test the interrupts */


	// clear_scr();
	// set_cur_pos(0);
	print_cstring("Hello, World!\n");


	__asm__ volatile("sti");
    init_timer(50);
    /* Comment out the timer IRQ handler to read
     * the keyboard IRQs easier */
    init_keyboard();

	 
	
}

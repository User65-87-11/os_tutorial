#include "timer.h"
#include "../drivers/screen.h"
#include "types.h"
#include "isr.h"
#include "../libc/strings.h"


u32 tick = 0;

static void timer_callback(registers_t regs) {
    tick++;

    print_cstring_at("Tick: ",0,0);
    
    char tick_ascii[15]={};

    itoa32(tick, tick_ascii,10);
	int len = 6 + str_len32(tick_ascii)+1;	

    print_cstring_at(tick_ascii,6,0);
    print_cstring_at(" ",len,0);
}

void init_timer(u32 freq) {
    /* Install the function we just wrote */
    register_interrupt_handler(IRQ0, timer_callback);

	

    /* Get the PIT value: hardware clock at 1193180 Hz */
    //u32 divisor = 100;
	u32 divisor = 1193180 / freq;
    u8 low  = (u8)(divisor & 0xFF);
    u8 high = (u8)( (divisor >> 8) & 0xFF);
    /* Send the command */
    port_byte_out(0x43, 0x36); /* Command port */
    port_byte_out(0x40, low);
    port_byte_out(0x40, high);
}

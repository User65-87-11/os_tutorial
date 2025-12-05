#include "keyboard.h"
#include "ports.h"
#include "../cpu/isr.h"
#include "screen.h"



static void keyboard_callback(registers_t regs) {
    /* The PIC leaves us the scancode in port 0x60 */
    u8 scancode = port_byte_in(0x60);
    char sc_ascii[10];
    itoa32(scancode, sc_ascii,16);
    print_cstring("Keyboard scancode: ");
    print_cstring(sc_ascii);
	print_cstring(", ");
    print_letter(scancode);
	print_cstring("\n");
}

void init_keyboard() {
   register_interrupt_handler(IRQ1, keyboard_callback); 
}

void print_letter(u8 scancode) {
    switch (scancode) {
        case 0x0:
            print_cstring("ERROR");
            break;
        case 0x1:
            print_cstring("ESC");
            break;
        case 0x2:
            print_cstring("1");
            break;
        case 0x3:
            print_cstring("2");
            break;
        case 0x4:
            print_cstring("3");
            break;
        case 0x5:
            print_cstring("4");
            break;
        case 0x6:
            print_cstring("5");
            break;
        case 0x7:
            print_cstring("6");
            break;
        case 0x8:
            print_cstring("7");
            break;
        case 0x9:
            print_cstring("8");
            break;
        case 0x0A:
            print_cstring("9");
            break;
        case 0x0B:
            print_cstring("0");
            break;
        case 0x0C:
            print_cstring("-");
            break;
        case 0x0D:
            print_cstring("+");
            break;
        case 0x0E:
            print_cstring("Backspace");
            break;
        case 0x0F:
            print_cstring("Tab");
            break;
        case 0x10:
            print_cstring("Q");
            break;
        case 0x11:
            print_cstring("W");
            break;
        case 0x12:
            print_cstring("E");
            break;
        case 0x13:
            print_cstring("R");
            break;
        case 0x14:
            print_cstring("T");
            break;
        case 0x15:
            print_cstring("Y");
            break;
        case 0x16:
            print_cstring("U");
            break;
        case 0x17:
            print_cstring("I");
            break;
        case 0x18:
            print_cstring("O");
            break;
        case 0x19:
            print_cstring("P");
            break;
		case 0x1A:
			print_cstring("[");
			break;
		case 0x1B:
			print_cstring("]");
			break;
		case 0x1C:
			print_cstring("ENTER");
			break;
		case 0x1D:
			print_cstring("LCtrl");
			break;
		case 0x1E:
			print_cstring("A");
			break;
		case 0x1F:
			print_cstring("S");
			break;
        case 0x20:
            print_cstring("D");
            break;
        case 0x21:
            print_cstring("F");
            break;
        case 0x22:
            print_cstring("G");
            break;
        case 0x23:
            print_cstring("H");
            break;
        case 0x24:
            print_cstring("J");
            break;
        case 0x25:
            print_cstring("K");
            break;
        case 0x26:
            print_cstring("L");
            break;
        case 0x27:
            print_cstring(";");
            break;
        case 0x28:
            print_cstring("'");
            break;
        case 0x29:
            print_cstring("`");
            break;
		case 0x2A:
			print_cstring("LShift");
			break;
		case 0x2B:
			print_cstring("\\");
			break;
		case 0x2C:
			print_cstring("Z");
			break;
		case 0x2D:
			print_cstring("X");
			break;
		case 0x2E:
			print_cstring("C");
			break;
		case 0x2F:
			print_cstring("V");
			break;
        case 0x30:
            print_cstring("B");
            break;
        case 0x31:
            print_cstring("N");
            break;
        case 0x32:
            print_cstring("M");
            break;
        case 0x33:
            print_cstring(",");
            break;
        case 0x34:
            print_cstring(".");
            break;
        case 0x35:
            print_cstring("/");
            break;
        case 0x36:
            print_cstring("Rshift");
            break;
        case 0x37:
            print_cstring("Keypad *");
            break;
        case 0x38:
            print_cstring("LAlt");
            break;
        case 0x39:
            print_cstring("Spc");
            break;
        default:
            /* 'keuyp' event corresponds to the 'keydown' + 0x80 
             * it may still be a scancode we haven't implemented yet, or
             * maybe a control/escape sequence */
            if (scancode <= 0x7f) {
                print_cstring("Unknown key down");
            } else if (scancode <= 0x39 + 0x80) {
                print_cstring("key up ");
                print_letter(scancode - 0x80);
            } else print_cstring("Unknown key up");
            break;
    }
	
}

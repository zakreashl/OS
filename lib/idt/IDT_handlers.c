#include "../stdlib/stdlib.h"
#include "IDT_handlers.h"
#include "../screen/screen_services.h"
#include "../io/io.h"

idt_entry idts[IDT_ENTRIES] __attribute__((aligned(16)));
idt_ptr idtr;

char scancode_to_ascii[] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0,
    0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0,
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, 0, 0, ' '
};


void idt_set_gate(uint8_t index, uint32_t offset) {
    idts[index].offset_low  = (uint16_t)(offset & 0xFFFF);
    idts[index].selector    = CS;
    idts[index].zero        = 0;
    idts[index].flags       = 0x8E;
    idts[index].offset_high = (uint16_t)(offset >> 16);
}

void fault_handler(registers_t* regs) {
    print("EXCEPTION", VGA_COLOR_ERROR);

    // Later replace these with your actual int/hex print funcs
    // print_hex(0, 1, regs->int_no, VGA_COLOR_ERROR);
    // print_hex(0, 2, regs->err_code, VGA_COLOR_ERROR);
    // print_hex(0, 3, regs->eip, VGA_COLOR_ERROR);

    while (1) {
        __asm__ volatile("cli; hlt");
    }
}

void H_divide_by_zero() {
    add_error("Divide by zero error");
        
    while(1) {
        __asm__ volatile ("cli; hlt");
    }
}

void H_double_fault() {
    add_error("Double fault");
        
    while(1) {
        __asm__ volatile ("cli; hlt");
    }
}

void H_protection_fault() {
    add_error("Protection fault");

    while(1) {
        __asm__ volatile ("cli; hlt");
    }
}

void H_page_fault() {
    add_error("Page fault");
        
    while(1) {
        __asm__ volatile ("cli; hlt");
    }
}

void H_exception() {
    add_error("Unhandled exception");
        
    while(1) {
        __asm__ volatile ("cli; hlt");
    }
}

void H_no_error_code(int regs) {
    add_error("Unhandled exception NEC");
        
    while(1) {
        __asm__ volatile ("cli; hlt");
    }
}

void H_timer() {
    outb(0x20, 0x20);
}

void H_keyboard() {
    uint8_t scancode = inb(0x60);  // read the key

    if(!(scancode & 0x80)) { // Key release
        char ascii_char = scancode_to_ascii[scancode];

        if (ascii_char != 0) {
            char s[2];
            s[0] = ascii_char;
            s[1] = '\0';

            print(s, VGA_COLOR_INFO);

            if(ascii_char == '/') bleh();
            if(ascii_char == '.') blehh();
        }
    }

    outb(0x20, 0x20); 
}
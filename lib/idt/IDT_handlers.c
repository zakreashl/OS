#include "../stdlib/stdlib.h"
#include "IDT_handlers.h"
#include "../screen/screen_services.h"

idt_entry idts[IDT_ENTRIES];
idt_ptr idtr;

char scancode_to_ascii[] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0,
    0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0,
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, 0, 0, ' '
};

void outb(unsigned short port, unsigned char value) {
    asm("outb %1, %0" : : "dN"(port), "a"(value));
}

void remap_pic() {
    // Initialize both PICs
    outb(0x20, 0x11);  // master
    outb(0xA0, 0x11);  // slave

    // Set vector offsets
    outb(0x21, 0x20);  // master IRQs start at 32
    outb(0xA1, 0x28);  // slave IRQs start at 40

    // Tell PICs how they're wired together
    outb(0x21, 0x04);
    outb(0xA1, 0x02);

    // Set x86 mode
    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    // Unmask only IRQ1 (keyboard), mask everything else
    outb(0x21, 0xFD);
    outb(0xA1, 0xFF);
}

unsigned char inb(unsigned short port) {
    unsigned char value;
    asm("inb %1, %0" : "=a"(value) : "dN"(port));
    return value;
}

void idt_set_gate(uint8_t index, int offset) {
    idts[index].offset_high = (short) (offset >> 16);
    idts[index].offset_low =  (short) (offset & 0xFFFF);

    idts[index].flags = 0x8E;
    idts[index].selector = CS;
}

void H_divide_by_zero() {
    add_error("Divide by zero error");
    asm("cli; hlt");
}

void H_double_fault() {
    add_error("Double fault");
    asm("cli; hlt");
}

void H_protection_fault() {
    add_error("Protection fault");
    asm("cli; hlt");
}

void H_page_fault() {
    add_error("Page fault");
    asm("cli; hlt");
}

void H_exception() {
    add_error("Unhandled exception");
    asm("cli; hlt");
}

void H_timer() {
    outb(0x20, 0x20);
}

void H_keyboard() {
    unsigned char scancode = inb(0x60);  // read the key
    char ascii_char[] = {scancode_to_ascii[scancode], '\0'};

    print(1,3, (const char*)ascii_char, VGA_COLOR_SUCCESS);

    outb(0x20, 0x20);  // EOI — tell master PIC we're done
    asm("sti");
}
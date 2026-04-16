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

void H_no_error_code() {
    add_error("Unhandled exception NEC");
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
}
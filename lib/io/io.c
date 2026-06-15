#include "io.h"

void outb(uint16_t port, uint8_t value) {
    asm("outb %1, %0" : : "dN"(port), "a"(value));
}

uint8_t inb(uint16_t port) {
    unsigned char value;
    asm("inb %1, %0" : "=a"(value) : "dN"(port));
    return value;
}

void io_wait() {
    outb(0x80, 0x00);  // port 0x80 is unused, write is just a delay
}

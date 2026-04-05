#include "pic.h"
#include "io.h"

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

    // Unmask IRQ0 (timer) and IRQ1 (keyboard), mask everything else
    outb(0x21, 0xFC);
    outb(0xA1, 0xFF);
}

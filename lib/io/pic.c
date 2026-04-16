#include "pic.h"
#include "io.h"
#include "../screen/screen_services.h"

void io_wait() {
    outb(0x80, 0x00);  // port 0x80 is unused, write is just a delay
}

void remap_pic() {
    print(0,3,"1", VGA_COLOR_SUCCESS);
    outb(0x20, 0x11); io_wait();
    print(2,3,"2", VGA_COLOR_SUCCESS);
    outb(0xA0, 0x11); io_wait();

    print(4,3,"3", VGA_COLOR_SUCCESS);
    outb(0x21, 0x20); io_wait();
    print(6,3,"4", VGA_COLOR_SUCCESS);
    outb(0xA1, 0x28); io_wait();

    outb(0x21, 0x04); io_wait();
    outb(0xA1, 0x02); io_wait();

    outb(0x21, 0x01); io_wait();
    outb(0xA1, 0x01); io_wait();

    outb(0x21, 0xFC);
    outb(0xA1, 0xFF);
}

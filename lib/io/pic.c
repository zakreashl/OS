#include "pic.h"
#include "io.h"
#include "../screen/screen_services.h"

void remap_pic() {
    print(0,0,"1", VGA_COLOR_SUCCESS);
    outb(0x20, 0x11); 
    io_wait();
    
    print(2,0,"2", VGA_COLOR_SUCCESS);
    outb(0xA0, 0x11); 

    print(3,0,"3", VGA_COLOR_SUCCESS);
    outb(0x21, 0x20); 
    print(4,0,"4", VGA_COLOR_SUCCESS);
    outb(0xA1, 0x28); 

    outb(0x21, 0x04); 
    outb(0xA1, 0x02); 

    outb(0x21, 0x01); 
    outb(0xA1, 0x01); 

    outb(0x21, 0xFC);
    outb(0xA1, 0xFF);
}

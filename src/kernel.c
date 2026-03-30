void _start() {
    char *vga = (char*)0xB8000;
    vga[0] = 'H';
    vga[1] = 0x07;  // color: white on black

    while(1); // hang forever so it doesn't fall off the end
}
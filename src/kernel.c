volatile char *vga = (volatile char*)0xB8000;

void clear_screen() {
    for(int x = 0; x < 80 * 25; x++) {
        vga[x * 2] = ' ';
        vga[x * 2 + 1] = 0x07;
    }
}

void print(int x, int y, const char* s, int color) {
    while(*s != '\0') {
        int offset = (y * 80 + x) * 2;
        vga[offset] = *s;
        vga[offset + 1] = color;
        x++;
        s++;
    }
}

void kernel_main() {
    clear_screen();
    print(25, 11, "Hello World", 0x07);
    while(1);
}
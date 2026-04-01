#include "screen_services.h"
#include "../string/string.h"

volatile char *vga = (volatile char*)0xB8000;
char error_buffer[ERROR_BUFF_SIZE];

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

void add_error(const char* s) {
    if(strlen(s) + strlen(error_buffer) > ERROR_BUFF_SIZE) return;
    strcat(error_buffer, " ( ");
    strcat(error_buffer, s);
    strcat(error_buffer, " ) ");

    int line = SCREEN_HEIGHT - 1 - (strlen(error_buffer)/SCREEN_WIDTH);
    print(0, line, error_buffer, VGA_COLOR_ERROR);
}
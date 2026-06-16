#include "screen_services.h"
#include "../string/string.h"
#include "../io/io.h"

// Globals

volatile uint16_t *vga = (volatile uint16_t*)0xB8000;

terminal_t* displayed_terminal;

terminal_t shell_terminal;
terminal_t error_terminal;

// Helpers functions

// Used to convert a terminal_cell struct to a uint16
static inline uint16_t vga_entry(terminal_cell cell) {
    return (uint16_t)cell.ch | ((uint16_t)cell.color << 8);
}

// Printing and stuff

void clear_screen() {
    for(int x = 0; x < 80 * 25; x++) {
        vga[x * 2] = ' ';
        vga[x * 2 + 1] = VGA_COLOR_DEFAULT;
    }
}

void print_to_terminal(terminal_t* terminal, const char* s, int color) {
    int len = strlen(s);

    for(int i = 0; i < len; i++) {
        terminal->terminal_end->ch = s[i];
        terminal->terminal_end->color = color;
        terminal->terminal_end++;
    }

    move_cursor_forward(terminal, len);
    set_cursor_location(terminal, terminal->cursor_x, terminal->cursor_y);
    change_cursor(terminal, CURSOR_BLOCK);
}

void print(const char* s, int color) {
    int len = strlen(s);

    for(int i = 0; i < len; i++) {
        displayed_terminal->terminal_end->ch = s[i];
        displayed_terminal->terminal_end->color = color;
        displayed_terminal->terminal_end++;
    }

    move_cursor_forward(displayed_terminal, len);
    set_cursor_location(displayed_terminal, displayed_terminal->cursor_x, displayed_terminal->cursor_y);
    change_cursor(displayed_terminal, CURSOR_BLOCK);
    display_terminal();
    display_cursor();
}

void add_error(const char* s) {
    print_to_terminal(&error_terminal, s, VGA_COLOR_ERROR);
}

// Terminal functions

void terminal_system_init() {
    terminal_init(&shell_terminal);
    terminal_init(&error_terminal);

    set_displayed_terminal(&shell_terminal);

    clear_screen();
    display_terminal();
    display_cursor();
}

void terminal_init(terminal_t* terminal) {
    terminal->cursor_x = 0;
    terminal->cursor_y = 0;
    terminal->cursor_type = CURSOR_UNDERLINE;

    for(int i = 0; i < TERMINAL_HEIGHT * SCREEN_WIDTH; i++) {
        terminal->cells[i].ch = ' ';
        terminal->cells[i].color = VGA_COLOR_DEFAULT;
    }

    terminal->terminal_end   = &terminal->cells[0];
    terminal->terminal_start = &terminal->cells[0];

    
    change_cursor(terminal, CURSOR_BLOCK);
    set_cursor_location(terminal, 0, 0);
}

void set_displayed_terminal(terminal_t* terminal) {
    displayed_terminal = terminal;
}

void display_terminal() {
    for(int i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH; i++) {
        vga[i] = vga_entry(displayed_terminal->cells[i]);
    }
}

// Cursor functions

void set_cursor_location(terminal_t* terminal, int x, int y) {
    terminal->cursor_x = x;
    terminal->cursor_y = y;
}

void display_cursor() {
    // Extract the top 8 bits for the start register
    uint8_t cursor_start = (displayed_terminal->cursor_type >> 8) & 0xFF;

    // Extract the bottom 8 bits for the end register
    uint8_t cursor_end   = displayed_terminal->cursor_type & 0xFF;

    // Write to Cursor Start Register
    outb(0x3D4, 0x0A);
    outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);

    // Write to Cursor End Register
    outb(0x3D4, 0x0B);
    outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);

    uint16_t pos = displayed_terminal->cursor_y * SCREEN_WIDTH + displayed_terminal->cursor_x;

    // Send the high byte
    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char)(pos >> 8));
    
    // Send the low byte
    outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char)(pos & 0xFF));
}

void move_cursor_forward(terminal_t* terminal, int times) {
    for(int i = 0; i < times; i++) {
        terminal->cursor_x++;

        if(terminal->cursor_x >= SCREEN_WIDTH) {
            terminal->cursor_y++;
            terminal->cursor_x -= SCREEN_WIDTH;
        }
    }
}

void change_cursor(terminal_t* terminal, CursorType type) {
    terminal->cursor_type = type;
}

void bleh() {
    set_displayed_terminal(&error_terminal);
    display_terminal();
    display_cursor();
}

void blehh() {
    set_displayed_terminal(&shell_terminal);
    display_terminal();
    display_cursor();
}
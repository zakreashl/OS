#ifndef SCREEN_SERVICES_H
#define SCREEN_SERVICES_H

#include "../stdlib/stdlib.h"

/* VGA Foreground/Background Colors */
#define VGA_BLACK           0x0
#define VGA_BLUE            0x1
#define VGA_GREEN           0x2
#define VGA_CYAN            0x3
#define VGA_RED             0x4
#define VGA_MAGENTA         0x5
#define VGA_BROWN           0x6
#define VGA_LIGHT_GREY      0x7
#define VGA_DARK_GREY       0x8
#define VGA_LIGHT_BLUE      0x9
#define VGA_LIGHT_GREEN     0xA
#define VGA_LIGHT_CYAN      0xB
#define VGA_LIGHT_RED       0xC
#define VGA_LIGHT_MAGENTA   0xD
#define VGA_YELLOW          0xE
#define VGA_WHITE           0xF

/* VGA Attribute Bytes (bg << 4 | fg) */
#define VGA_COLOR_DEFAULT       0x07  /* Light grey on black    - normal text    */
#define VGA_COLOR_BRIGHT        0x0F  /* White on black         - emphasis       */
#define VGA_COLOR_ERROR         0x4F  /* White on red           - errors         */
#define VGA_COLOR_WARN          0x0E  /* Yellow on black        - warnings       */
#define VGA_COLOR_SUCCESS       0x0A  /* Light green on black   - success        */
#define VGA_COLOR_INFO          0x0B  /* Light cyan on black    - info           */
#define VGA_COLOR_HIGHLIGHT     0x70  /* Black on light grey    - highlighted    */
#define VGA_COLOR_HEADER        0x1F  /* White on blue          - header/title   */
#define VGA_COLOR_DEBUG         0x0D  /* Light magenta on black - debug          */
#define VGA_COLOR_CRITICAL      0xCF  /* White on light red     - critical       */


#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

#define TERMINAL_HEIGHT 1000

#define ERROR_BUFF_SIZE 2048


typedef enum {
    CURSOR_BLOCK     = 0x000F,  // Full block shape
    CURSOR_UNDERLINE = 0x0E0F,  // Standard underline
    CURSOR_HALF      = 0x070F,  // Bottom half block
    CURSOR_DISABLED  = 0x2000   // Turns the cursor completely off
} CursorType;

typedef struct {
    char ch;
    uint8_t color;
} terminal_cell;

typedef struct {
    terminal_cell cells[TERMINAL_HEIGHT * SCREEN_WIDTH]; // All the chars in this terminal

    int cursor_x; // x and y position of the cursor (used to tell vga where to display the terminal)
    int cursor_y;
    int cursor_index; // the index where the cursor is actually on

    CursorType cursor_type;

    terminal_cell* terminal_start; // the window of the terminal that is actually being displayed
    terminal_cell* terminal_end;
} terminal_t;


extern volatile uint16_t *vga;

extern char error_buffer[];

extern terminal_t* displayed_terminal;

extern terminal_t shell_terminal;
extern terminal_t error_terminal;


// Printing and stuff

void clear_screen();
void print(const char* s, int color);
void add_error(const char* s);


// Terminal functions

void terminal_system_init();
void terminal_init(terminal_t* terminal);
void set_displayed_terminal(terminal_t* terminal);
void display_terminal();
void switch_between_error_shell_terminal();


// Cursor functions

void set_cursor_location(terminal_t* terminal, int x, int y);
void move_cursor_forward(terminal_t* terminal, int times);
void display_cursor();
void change_cursor(terminal_t* terminal, CursorType type);

#endif
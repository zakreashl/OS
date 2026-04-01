#ifndef SCREEN_SERVICES_H
#define SRCEEN_SERVICES_H

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

#define ERROR_BUFF_SIZE 2048

extern volatile char *vga;

extern char error_buffer[];

void clear_screen();

void print(int x, int y, const char* s, int color);

void add_error(const char* s);

#endif
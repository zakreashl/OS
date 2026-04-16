#ifndef GDT_H
#define GDT_H

#include "../stdlib/stdlib.h"

typedef struct {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_middle;
    uint8_t  access;
    uint8_t  granularity;
    uint8_t  base_high;
} __attribute__((packed)) gdt_entry;

typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) gdt_ptr;

// Declare them so gdt.c can define them and other files can see them
extern void gdt_install();

#endif
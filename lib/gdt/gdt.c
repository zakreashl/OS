#include "gdt.h"

gdt_entry gdt[3];
gdt_ptr gdt_p;

// Internal function to set entry values
void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt[num].base_low    = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high   = (base >> 24) & 0xFF;

    gdt[num].limit_low   = (limit & 0xFFFF);
    gdt[num].granularity = (limit >> 16) & 0x0F;

    gdt[num].granularity |= gran & 0xF0;
    gdt[num].access      = access;
}

void gdt_install() {
    gdt_p.limit = (sizeof(gdt_entry) * 3) - 1;
    gdt_p.base  = (uint32_t)&gdt;

    gdt_set_gate(0, 0, 0, 0, 0);                
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); 
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); 

    __asm__ volatile (
        "lgdt %0\n\t"            // Load the new GDT
        "pushl $0x08\n\t"        // Push new CS (0x08)
        "pushl $1f\n\t"          // Push address of label '1'
        "lretl\n\t"              // "Far Return" - pops CS and EIP to reload them
        "1:\n\t"                 // We are now officially using the new GDT
        "mov $0x10, %%ax\n\t"    // Reload all data segments with 0x10
        "mov %%ax, %%ds\n\t"
        "mov %%ax, %%es\n\t"
        "mov %%ax, %%fs\n\t"
        "mov %%ax, %%gs\n\t"
        "mov %%ax, %%ss\n\t"
        : 
        : "m" (gdt_p)
        : "eax", "memory"
    );
}
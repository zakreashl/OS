#include "../../lib/stdlib/stdlib.h"
#include "../../lib/screen/screen_services.h"
#include "../../lib/idt/IDT_handlers.h"
#include "../../lib/io/pic.h"
#include "../../lib/io/io.h"
#include "../../lib/pmm/pmm.h"
#include "../../lib/gdt/gdt.h"

void set_IDT_table() {
    for(int i = 0; i < IDT_ENTRIES; i++) {
        idt_set_gate(i,  (uint32_t) H_no_error_code);  
    }

    idt_set_gate(0,   (uint32_t) H_divide_by_zero_stub);
    idt_set_gate(8,   (uint32_t) H_double_fault_stub);
    idt_set_gate(10,  (uint32_t) H_no_error_code);
    idt_set_gate(11,  (uint32_t) H_no_error_code);
    idt_set_gate(12,  (uint32_t) H_no_error_code);
    idt_set_gate(13,  (uint32_t) H_protection_fault_stub);
    idt_set_gate(14,  (uint32_t) H_page_fault_stub);

    idt_set_gate(32, (uint32_t) H_timer_stub);
    idt_set_gate(33, (uint32_t) H_keyboard_stub);

    idtr.base =  (uint32_t)&idts;
    idtr.limit = sizeof(idt_entry) * IDT_ENTRIES - 1;

    //__asm__ volatile("lidt (%0)" : : "r" (&idtr));
}

void kernel_main() {
    __asm__ volatile("cli");       // 1. Kill interrupts immediately
    
    gdt_install();                 // 2. Load a fresh, C-managed GDT
    set_IDT_table();               // 3. Setup and 'lidt' your IDT
    
    clear_screen();
    print(0, 0, "System Core Ready", VGA_COLOR_SUCCESS);

    remap_pic();                   // 4. Remap PIC while shielded by 'cli'
    
    // Mask EVERYTHING except the keyboard (0xFD = 11111101)
    outb(0x21, 0xFD);
    outb(0xA1, 0xFF);

    __asm__ volatile("sti");       // 5. Finally, open the gates
    while(1) __asm__ volatile("hlt");
}
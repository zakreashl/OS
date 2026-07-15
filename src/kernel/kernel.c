#include "../../lib/stdlib/stdlib.h"
#include "../../lib/screen/screen_services.h"
#include "../../lib/idt/IDT_handlers.h"
#include "../../lib/io/pic.h"
#include "../../lib/io/io.h"
#include "../../lib/pmm/pmm.h"
#include "../../lib/gdt/gdt.h"
#include "../../lib/timer/timer.h"
#include "../../lib/timer/timer.h"

void set_IDT_table() {
    for(int i = 0; i < IDT_ENTRIES; i++) {
        idt_set_gate(i,  (uint32_t) H_no_error_code_stub);
    }

    idt_set_gate(0,   (uint32_t) H_divide_by_zero_stub);
    idt_set_gate(8,   (uint32_t) H_double_fault_stub);
    idt_set_gate(10,  (uint32_t) H_no_error_code_stub);
    idt_set_gate(11,  (uint32_t) H_no_error_code_stub);
    idt_set_gate(12,  (uint32_t) H_no_error_code_stub);
    idt_set_gate(13,  (uint32_t) H_protection_fault_stub);
    idt_set_gate(14,  (uint32_t) H_page_fault_stub);

    idt_set_gate(32,  (uint32_t) H_timer_stub);
    idt_set_gate(33,  (uint32_t) H_keyboard_stub);

    idtr.base =  (uint32_t)&idts;
    idtr.limit = sizeof(idt_entry) * IDT_ENTRIES - 1;

    __asm__ volatile("lidt (%0)" : : "r" (&idtr));
}

void kernel_main() {
    __asm__ volatile("cli");
    
    gdt_install();
    set_IDT_table();

    terminal_system_init();
    add_timer_callback(&display_terminal, 1);
    add_timer_callback(&display_cursor, 1);
    //for(int i = 0; i < 100; i++) {
    //    print("System Core Ready", VGA_COLOR_SUCCESS);
    //}

    remap_pic();

    __asm__ volatile("sti");
    while(1) __asm__ volatile("hlt");
}
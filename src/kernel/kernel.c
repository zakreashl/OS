#include "../../lib/stdlib/stdlib.h"
#include "../../lib/screen/screen_services.h"
#include "../../lib/idt/IDT_handlers.h"

void set_IDT_table() {
    for(int i = 0; i < IDT_ENTRIES; i++) {
        idt_set_gate(i,  (uint32_t) H_exception);  
    }

    idt_set_gate(0,  (uint32_t) H_divide_by_zero);
    idt_set_gate(8,  (uint32_t) H_double_fault);
    idt_set_gate(13, (uint32_t) H_protection_fault);
    idt_set_gate(14, (uint32_t) H_page_fault);

    idt_set_gate(32, (uint32_t) H_timer_stub);
    idt_set_gate(33, (uint32_t) H_keyboard_stub);

    idtr.base =  (uint32_t)&idts;
    idtr.limit = sizeof(idt_entry) * IDT_ENTRIES - 1;

    asm("lidt %0" :: "m"(idtr));
    asm("sti");
}

void kernel_main() {
    remap_pic();
    set_IDT_table();
    clear_screen();
    print(0, 0, "Hello World", VGA_WHITE);
    while(1);
}
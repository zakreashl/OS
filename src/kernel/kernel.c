#include "../../lib/stdlib/stdlib.h"
#include "../../lib/screen/screen_services.h"
#include "../../lib/idt/IDT_handlers.h"
#include "../../lib/io/pic.h"
#include "../../lib/pmm/pmm.h"

void set_IDT_table() {
    for(int i = 0; i < IDT_ENTRIES; i++) {
        idt_set_gate(i,  (uint32_t) H_exception_stub);  
    }

    idt_set_gate(0,  (uint32_t) H_divide_by_zero_stub);
    idt_set_gate(8,  (uint32_t) H_double_fault_stub);
    idt_set_gate(13, (uint32_t) H_protection_fault_stub);
    idt_set_gate(14, (uint32_t) H_page_fault_stub);

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

    MemMapEntry *map = (MemMapEntry *)0x500; // Where we told BIOS to put it in stage2.s
    uint16_t     count = *(uint16_t *)0x4FC; // Where BIOS just put the count
    pmm_init(map, count);
    pmm_mark_used(0, 0x100000); // Mark first 1MB used, BIOS, bootloader and kernel

    clear_screen();
    print(0, 0, "Hello World", VGA_WHITE);
    while(1);
}
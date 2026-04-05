#ifndef IDT_HANDLERS_H
#define IDT_HANDLERS_H

#define IDT_ENTRIES 256

typedef struct {
    short offset_low;   // lower 16 bits of handler address
    short selector;     // code segment (0x08 in your GDT)
    char  zero;         // always 0
    char  flags;        // 0x8E = present, kernel, 32-bit interrupt gate
    short offset_high;  // upper 16 bits of handler address
} __attribute__((packed)) idt_entry;

typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_ptr;

// __attribute__((packed)) so gcc doesn't add any padding

extern idt_entry idts[];
extern idt_ptr idtr;

extern char scancode_to_ascii[];

void idt_set_gate(uint8_t index, int offset);

void H_divide_by_zero();
extern void H_divide_by_zero_stub();
void H_double_fault();
extern void H_double_fault_stub();
void H_protection_fault();
extern void H_protection_fault_stub();
void H_page_fault();
extern void H_page_fault_stub();

void H_exception();
extern void H_exception_stub();

void H_timer();
extern void H_timer_stub();

void H_keyboard();
extern void H_keyboard_stub();


#endif
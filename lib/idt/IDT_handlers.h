#ifndef IDT_HANDLERS_H
#define IDT_HANDLERS_H

#define IDT_ENTRIES 256

typedef struct {
    uint16_t offset_low;   // lower 16 bits of handler address
    uint16_t selector;     // code segment (0x08)
    uint8_t  zero;         // always 0
    uint8_t  flags;        // 0x8E
    uint16_t offset_high;  // upper 16 bits of handler address
} __attribute__((packed)) idt_entry;

typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_ptr;

// Whenever we get an exception the we can get all the CPU's registers which contains error codes which help with debugging a lot
typedef struct registers {
    uint32_t gs, fs, es, ds;

    uint32_t edi, esi, ebp, esp;
    uint32_t ebx, edx, ecx, eax;

    uint32_t int_no;
    uint32_t err_code;

    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
} __attribute__((packed)) registers_t;

// __attribute__((packed)) so gcc doesn't add any padding

extern idt_entry idts[];
extern idt_ptr idtr;

extern char scancode_to_ascii[];

void idt_set_gate(uint8_t index, uint32_t offset);

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
void H_no_error_code(int regs);
extern void H_no_error_code_stub(int regs);

void H_keyboard();
extern void H_keyboard_stub();


#endif
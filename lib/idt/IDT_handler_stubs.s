[bits 32]

[global H_divide_by_zero_stub]
[extern H_divide_by_zero]
H_divide_by_zero_stub:
    pusha
    call H_divide_by_zero
    popa
    iret

[global H_double_fault_stub]
[extern H_double_fault]
H_double_fault_stub:
    add esp, 4      ; discard error code pushed by CPU
    pusha
    call H_double_fault
    popa
    iret

[global H_protection_fault_stub]
[extern H_protection_fault]
H_protection_fault_stub:
    add esp, 4      ; discard error code pushed by CPU
    pusha
    call H_protection_fault
    popa
    iret

[global H_page_fault_stub]
[extern H_page_fault]
H_page_fault_stub:
    add esp, 4      ; discard error code pushed by CPU
    pusha
    call H_page_fault
    popa
    iret

[global H_exception_stub]
[extern H_exception]
H_exception_stub:
    pusha
    call H_exception
    popa
    iret

[global H_keyboard_stub]
[extern H_keyboard]
H_keyboard_stub:
    pusha
    call H_keyboard
    popa
    iret

[global H_timer_stub]
[extern H_timer]
H_timer_stub:
    pusha
    call H_timer
    popa
    iret
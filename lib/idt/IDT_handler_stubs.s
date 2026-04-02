[bits 32]


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
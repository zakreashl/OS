[bits 32]


[global H_keyboard_stub]
[extern H_keyboard]
H_keyboard_stub:
    pusha
    call H_keyboard    ; your C function, no iret in it
    popa
    iret
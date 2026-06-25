[bits 16]
[org 0x7c00]
jmp 0x0000:start

start:
    cli ; Disable interrupts
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
    sti ; Enable interrupts

    jmp load_kernel

load_kernel:
    mov ax, 0x1000
    mov es, ax          ; ES = 0x1000
    xor bx, bx          ; BX = 0, so ES:BX = 0x10000

    mov al, 8           ; 8 sectors for the kernel
    mov ch, 0           ; cylinder 0
    mov cl, 2           ; kernel starts at sector 2
    mov dh, 0           ; head 0
    mov dl, 0x80        ; which hard drive to access 
    mov ah, 0x02
    int 0x13

    jc disk_error

    jmp enable_A20

enable_A20:
    in al, 0x92
    or al, 2
    out 0x92, al

    jmp load_gdt

gdt_start:
    dd 0x00000000
    dd 0x00000000

    ; Code segment descriptor
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10011010b
    db 11001111b
    db 0x00

    ; Data segment descriptor
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10010010b
    db 11001111b
    db 0x00
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

load_gdt:
    cli
    lgdt [gdt_descriptor]
    call get_memory_map
    jmp load_PM

get_memory_map:
    xor ax, ax
    mov es, ax          ; ES:DI = 0x0000:0x0500 = physical 0x500
    
    mov di, 0x500           ; where to store entries
    xor ebx, ebx            ; ebx = 0 to start
    xor bp, bp              ; entry count = 0

.loop:
    mov eax, 0xE820
    mov edx, 0x534D4150     ; "SMAP" magic number the BIOS expects
    mov ecx, 24             ; 24 bytes per entry
    int 0x15

    jc .done                ; carry flag set = no more entries
    inc bp
    add di, 24
    test ebx, ebx           ; ebx = 0 means last entry
    jnz .loop

.done:
    mov [0x4FC], bp         ; store count just before the map
    ret

load_PM:
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax

    jmp 0x08:pm_entry

[bits 32]
pm_entry:
    mov ax, 0x10        ; data segment selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x200000

    jmp 0x10000         ; jump to kernel



disk_error:
    hlt
    jmp disk_error

times 510 - ($ - $$) db 0
dw 0xAA55

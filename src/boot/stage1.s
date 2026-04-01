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

    ; load stage 2
    mov ah, 0x02      ; function: read sectors
    mov al, 5         ; number of sectors to read
    mov ch, 0         ; cylinder 0
    mov cl, 2         ; start at sector 2 (1-indexed, sector 1 is the MBR)
    mov dh, 0         ; head 0
    mov dl, 0x80      ; drive 0x80 = first hard disk (0x00 = floppy)
    mov bx, 0x8000    ; load data into memory at 0x8000
    int 0x13

    jc disk_error

    jmp 0x8000

disk_error:
    hlt
    jmp disk_error

times 510 - ($ - $$) db 0
dw 0xAA55

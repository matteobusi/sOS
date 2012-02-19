global gdt_flush

gdt_flush:
    mov eax, [esp+4]
    lgdt [eax]
    mov ax, 0x10 ; the address in memory of gdt data segment
    mov ds, ax 
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:.flush 

.flush:
    ret

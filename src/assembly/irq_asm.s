[bits 32]

; This macro creates a stub for an IRQ
%macro IRQ 2
  global irq%1
  irq%1:
    cli
    push byte 0
    push byte %2
    jmp irq_common_handler
%endmacro

IRQ 1, 33
IRQ 2, 34
IRQ 3, 35
IRQ 4, 36
IRQ 5, 37
IRQ 6, 38
IRQ 7, 39
IRQ 8, 40
IRQ 9, 41
IRQ 10,42
IRQ 11,43
IRQ 12,44
IRQ 13,45
IRQ 14,46
IRQ 15,47

;this goes in irq.c
extern irq_handler

irq_common_handler:
    pusha ;saves all 32bit registers

    mov ax, ds
    push eax

    mov ax, 0x10 ;load kernel's data segment descriptor from gdt
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call irq_handler

    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa
    add esp,8
    sti
    iret

extern schedule ;scheduling function..
[GLOBAL irq0] ;irq0 requires a special handler...
irq0:
                pusha           ;push general registers onto stack
                push ds         ;push ds on the stack
                push es         ;push es
                push fs         ;push fs
                push gs         ;push gs

                mov eax, 0x10   ;saves selector
                mov ds, eax
                mov es, eax
                mov fs, eax
                mov gs, eax

                push esp        ;pushes esp

                call schedule   ;schedule(eax) returns stack of next task
                mov esp, eax    ;return value

                mov al, 0x20    ;ack IRQ
                out 0x20, al

                pop gs          ;pop gs off task stack
                pop fs          ;pop fs
                pop es          ;pop es
                pop ds          ;pop ds
                popa            ;pop general registers

                iret            ;interrupt return
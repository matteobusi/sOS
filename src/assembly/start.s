;We are in 32bit mode.
;Thanks, GRUB :)
[BITS 32]

global start
;Start function
start:
    mov esp, _sys_stack+0x10000 ;the ESP registers contains _sys_stack
    jmp stublet

align 4
;This kernel is Multiboot compliant!
;for reference http://www.gnu.org/software/grub/manual/multiboot/multiboot.html#Header-layout
mboot:
    ;multiboot flags
    MULTIBOOT_PAGE_ALIGN	equ 1<<0
    MULTIBOOT_MEMORY_INFO	equ 1<<1
    MULTIBOOT_AOUT_KLUDGE	equ 1<<16

    ;multiboot headers
    MULTIBOOT_HEADER_MAGIC equ 0x1BADB002
    MULTIBOOT_HEADER_FLAGS equ MULTIBOOT_PAGE_ALIGN	| MULTIBOOT_MEMORY_INFO | MULTIBOOT_AOUT_KLUDGE
    MULTIBOOT_HEADER_CHECKSUM equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

    extern code, bss, kend

    dd MULTIBOOT_HEADER_MAGIC
    dd MULTIBOOT_HEADER_FLAGS
    dd MULTIBOOT_HEADER_CHECKSUM

    dd mboot
    dd code
    dd bss
    dd kend
    dd start

stublet:
    ;Calls the kernel main function
    extern kmain
    push ebx
    push eax
    call kmain
    jmp $
section .bss
_sys_stack:
    resb 0x10000 ;16KB stack

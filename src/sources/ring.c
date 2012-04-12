#include <userland/ring.h>

void switch_to_user()
{
    update_kstack(tasks->current->value->stack);
    //Well, these instructions perform ring switching in kernel
    //nothing too complicate, we move the data selector to 16-bit registers (ax, ds, es, fs, gs), we move
    //the stack pointer to eax and we push data selector, eax.
    //Now, after pushf, we setup eflags to reenable interrupts after our iret (see wikipedia why the eflag to eax
    
    asm volatile("\
    cli;       \
    mov $0x23, %ax;    \
    mov %ax, %ds;   \
    mov %ax, %es;   \
    mov %ax, %fs;   \
    mov %ax, %gs;   \
    mov %esp, %eax; \
    pushl $0x23;       \
    pushl %eax;     \
    pushf;          \
    pop %eax;       \
    or %eax, 0x200; \
    push %eax;      \
    pushl $0x1b;       \
    push $1f;       \
    iret;           \
    1:              \
    ");
}
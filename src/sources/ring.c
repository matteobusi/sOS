#include <userland/ring.h>

unsigned int current_level=KERNEL_RING;

unsigned int get_run_level()
{
    return current_level;
}

void switch_to_user()
{
    update_kstack(tasks->current->value->kernel_stack+STACK_SIZE);
    //Well, these instructions perform ring switching in kernel
    //nothing too complicate, we move the data selector to 16-bit registers (ax, ds, es, fs, gs), we move
    //the stack pointer to eax and we push data selector, eax.
    current_level=USER_RING;
    asm volatile("  mov $0x23, %ax; \
                    mov %ax, %ds;   \
                    mov %ax, %es;   \
                    mov %ax, %fs;   \
                    mov %ax, %gs;   \
                    mov %esp, %eax; \
                    pushl $0x23;    \
                    pushl %eax;     \
                    pushf;          \
                    pushl $0x1b;    \
                    push $1f;       \
                    iret;           \
                    1:              \
                    ");
}
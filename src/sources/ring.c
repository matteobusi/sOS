#include <userland/ring.h>

void switch_ring(unsigned short level)
{
    unsigned short code_selector=CODE_SELECTOR(level);
    unsigned short data_selector=DATA_SELECTOR(level);
    
    asm volatile("cli;       \
    mov %0, %%ax;    \
    mov %%ax, %%ds;   \
    mov %%ax, %%es;   \
    mov %%ax, %%fs;   \
    mov %%ax, %%gs;   \
                    \
    movl %%esp, %%eax; \
    pushl %1;       \
    pushl %%eax;     \
    pushf;          \
    pop %%eax;       \
    or %%eax, 0x200; \
    push %%eax;      \
    pushl %2;       \
    push $1f;       \
    iret;           \
    1:              \
    "::"r" (data_selector), "r" (data_selector), "r"(code_selector));
}
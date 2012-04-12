#include <interrupts/panic.h>


void kpanic(char* error_string, int address_violation, char* file, int line)
{
    //white on red
    set_color(4, 15);
    //clear();
    kprintf("*****ERROR - KERNEL PANIC!*****\n");
    kprintf("ERROR: %s\n", error_string);
    kprintf("At address: 0x%h\nIn file: %s:%d\n*****SYSTEM HALTED!*****\n\n", address_violation, file, line );
    asm volatile("cli;hlt");
}

void kexception(int err_no, int int_no)
{
    asm volatile("cli");
    //white on red
    set_color(4, 15);
    clear();
    kprintf("*****ERROR - NON-MANAGED EXCEPTION!*****\n");
    kprintf("EXCEPTION NO: 0x%h\nINTERRUPT NO: 0x%h\n*****SYSTEM HALTED!*****\n\n",err_no, int_no );
    for(;;);
    asm volatile("cli;hlt");
}

inline void debug()
{ asm volatile("cli;hlt");}
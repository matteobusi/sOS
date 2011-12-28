#include <interrupts/panic.h>


void kpanic(char* error_string, int address_violation, char* file, int line)
{
    //white on red
    set_color(4, 15);
    clear();
    kprintf("*****ERROR - KERNEL PANIC!*****\n");
    kprintf("ERROR: %s\n", error_string);
    kprintf("At address: 0x%h\nIn file: %s:%d\n*****SYSTEM HALTED!*****\n\n",error_string, address_violation, file, line );
    asm volatile("hlt");
}

void kexception(int err_no, int int_no)
{
    //white on red
    set_color(4, 15);
    clear();
    kprintf("*****ERROR - NON-MANAGED EXCEPTION!*****\n");
    kprintf("EXCEPTION NO: %d\nINTERRUPT NO: %d\n*****SYSTEM HALTED!*****\n\n",err_no, int_no );
    for(;;);
    asm volatile("hlt");
}

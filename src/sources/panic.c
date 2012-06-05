#include <interrupts/panic.h>

void kpanic(char* error_string, int address_violation, char* file, int line)
{
    //white on red
    set_color(4, 15);
    kprintf("\n*****ERROR - KERNEL PANIC!*****\n");
    kprintf("%s\nAddress: 0x%h (%u)\nFile: %s:%d\n*****SYSTEM HALTED!*****\n\n",error_string, address_violation, address_violation, file,line );
    asm volatile("cli;hlt");
}

void kexception(int err_no, int int_no)
{
    disable_int(__FILE__, __LINE__);
    //white on red
    set_color(4, 15);
    // clear();
    kprintf("*****ERROR - NON-MANAGED EXCEPTION!*****\n");
    kprintf("EXCEPTION NO: 0x%h (%u) \nINTERRUPT NO: 0x%h (%u)\n*****SYSTEM HALTED!*****\n\n",err_no, err_no, int_no,int_no );
    asm volatile("cli;hlt");
}

inline void debug()
{
    asm volatile("cli;hlt");
}
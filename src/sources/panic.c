
#include <panic.h>
#include <stdout.h>
#include <stdarg.h>

void kpanic(char* error_string, int address_violation, char* file, int line)
{
    //white on red
    set_color(4, 15);
    clear();
    kprintf("*****ERROR - KERNEL PANIC!*****\n");
    kprintf("ERROR: %s\nAt address: 0x%h (%u)\nIn file: %s:%d\n*****SYSTEM HALTED!*****\n\n",error_string, address_violation, address_violation, file, line );
    for(;;);
}

void kexception(int err_no, int int_no)
{
    //white on red
    set_color(4, 15);
    clear();
    kprintf("*****ERROR - NON-MANAGED EXCEPTION!*****\n");
    kprintf("EXCEPTION NO: %d\nINTERRUPT NO: %d\n*****SYSTEM HALTED!*****\n\n",err_no, int_no );
    for(;;);
}

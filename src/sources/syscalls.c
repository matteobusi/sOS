#include <interrupts/syscalls.h>

void start_calls()
{
    add_handler(0x80, syscall_mngr);
}
      
void syscall_mngr(struct registers r)
{
    //TODO: manage all informations
}
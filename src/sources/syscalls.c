#include <interrupts/syscalls.h>

extern struct page_dir* current_dir;
int is_call=0;

void start_calls()
{
    call_id=0;    
    add_handler(0x80, (isr_t)&syscall_mngr);
    add_call(&add_call); //0
    
    /* Temporary system calls, to be removed asavfs (as soon as virtual fs) */    
    add_call(&puts); // 1
    add_call(&putu); // 2
    
    add_call(&asyn_send); // 3
    add_call(&asyn_receive); // 4
    add_call(&get_pid); // 5
    add_call(&drv_reg_handler); // 6
    add_call(&inportb); // 7
    add_call(&outportb); // 8
    add_call(&putc); // 9
    
    /* Here minimal system call setup for NewLib */
    add_call(&wrap_exit); // 10   
        
    add_call(&fopen); // 11
    add_call(&fclose); // 12
    add_call(&fwrite); // 13 
    add_call(&fread); // 14
    
    add_call(&get_stdin); // 15
    add_call(&get_stdout); // 16
    add_call(&fgetc); // 17
    
    add_call(&get_fg_stdin); // 18
    add_call(&fputc); // 19
    add_call(&list); // 20
    add_call(&add_user_task); // 21
    add_call(&wait); // 22
    
    add_call(&get_x_cur); // 23
    add_call(&get_y_cur); // 24
    add_call(&set_cur_pos); // 25
    
    add_call(&get_fg_pid); // 26
    
    add_call(&kill); // 27
    add_call(&add_boot_task); // 28
}

int add_call(syscall_t call)
{
    calls_vector[call_id++]=call;  
    return call_id-1;
}

unsigned int syscall_mngr(struct registers* r)
{     
    int call_nr=r->eax;

    if(call_nr<0 || call_nr>=call_id)
        return -1; //non valid call!
    void* sys_call=calls_vector[call_nr];
       
    int exit_code=0;
    is_call=1;
    asm volatile ("\
        push %1;   \
        push %2;   \
        push %3;   \
        push %4;   \
        push %5;   \
        call *%6;  \
        pop %%ebx; \
        pop %%ebx; \
        pop %%ebx; \
        pop %%ebx; \
        pop %%ebx; \
   " : "=a" (exit_code) : "r" (r->edi), "r" (r->esi), "r" (r->edx), "r" (r->ecx), "r" (r->ebx), "r" (sys_call));
    is_call=0;
    return exit_code;
} 
int is_syscall()
{
    disable_int(__FILE__, __LINE__);
    int tmp=is_call;
    enable_int();
    return tmp;
}
void garbage_collect()
{
    return;
}
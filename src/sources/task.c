#include <multitasking.h>

struct list* tasks;
unsigned int pid=0;

extern struct page_dir* current_dir;
extern struct page_dir* kernel_dir;

void init_tasking()
{
    tasks=make_list();
    //adds the handler
    add_schedule_function(schedule);
}

unsigned int tick_count()
{
    return tick;
}


void print()
{ 

}
//TODO: Add priority queues
stack_t schedule(stack_t context)
{
    tick++;
    asm volatile("cli");
    //kprintf("%s\n", tasks->current->value->name);
    //kprintf("switching...");
    //if no tasks...
    if(tasks->size==0)
        return context;
       
    tasks->current->value->stack=context;
    next(tasks);
    if(tasks->current->value->state == ENDED)
        remove(tasks->current->value, tasks);
    
    
    load_dir(tasks->current->value->directory);
    
    asm volatile("sti");
    return tasks->current->value->stack;
}

inline void exit(int status)
{
    asm volatile("cli");

    struct task* current=tasks->current->value;
    current->ret_value=status;
    current->state=ENDED;
    
    kprintf("Process %s exited with status [%d]\n", current->name, current->ret_value);
    asm volatile("sti");
    
    for(;;); //now stop the process...it will be effectively deleted when the scheduler redo the context switch...
}


void add_task(char* name, int priority, run_t run, int argc, char** argv)
{
    if(name==(char*)NULL || strcmp(name, "")==0 || run==(run_t)NULL)
        return;
    
    asm volatile("cli"); //disable interrupts..

    unsigned int* stack;
    struct task* new_task=(struct task*)kmalloc(sizeof(struct task));
    
    new_task->argc=argc;
    new_task->argv=argv;
    
    stack =(unsigned int*)(kmalloc_a(4096));//allocates 4096bytes...
    if(stack == (unsigned int*)NULL)
        kpanic("Error while creating space for the new stack", 0x0, __FILE__, __LINE__);

    //because of the structure of x86, the stack grows downward, then
    //we set all data for task-switching...
    *--stack = 0x0202;
    *--stack = 0x08;        // CS - next instruction
    *--stack = (unsigned int)dummy;// EIP - this points to a function, to control the status of the task

    // pusha
    *--stack = 0;           // EDI
    *--stack = 0;           // ESI
    *--stack = 0;           // EBP
    *--stack = 0;           // NULL
    *--stack = 0;           // EBX
    *--stack = 0;           // EDX
    *--stack = 0;           // ECX
    *--stack = 0;           // EAX

    // data segments
    *--stack = 0x10;        // DS
    
    *--stack = 0x10;        // ES
    *--stack = 0x10;        // FS
    *--stack = 0x10;        // GS

    new_task->stack = (unsigned int)stack; //refreshes the position of the stack
    new_task->run = run;
    
    //setup a new dir-table-pages  
    
    
    new_task->directory = clone_directory(kernel_dir);//clone_directory(current_dir);
    
    strcpy(new_task->name, name);
    new_task->priority = priority;
    new_task->pid=pid++;
    new_task->state = READY; //ready to run
    add(new_task, tasks);
    
    asm volatile("sti"); //reenable interrupts...
}

void dummy()
{
    // Here we are sure we're in the right context, because current it's referring to ourselves
    // Not this time Philosophers!
    exit(tasks->current->value->run(tasks->current->value->argc, tasks->current->value->argv)); //Little tricky uh?    
}
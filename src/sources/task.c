#include <multitasking.h>

struct list* tasks;
unsigned int pid=0;

void init_tasking()
{
    tasks=make_list();
}

unsigned int tick_count()
{
    return tick;
}
void print()
{ 

}
//TODO: Add priority queues
unsigned int schedule(unsigned int context)
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
        remove(tasks->current, tasks);

    asm volatile("sti");
    return tasks->current->value->stack;
}

void clear_all()
{
    
}

void add_task(char* name, int priority, void (*run)())
{
    if(name==(char*)NULL || strcmp(name, "")==0 || run==NULL)
        return;
    
    asm volatile("cli"); //disable interrupts..

    unsigned int* stack;
    struct task* new_task=(struct task*)kmalloc(sizeof(struct task));

    stack =(unsigned int*)(kmalloc_a(STACK_SIZE) + STACK_SIZE);//allocates 4096bytes...
    if(stack == (unsigned int*)NULL)
        kpanic("Error while creating space for a new stack", 0x0, __FILE__, __LINE__);

    //because of the structure of x86, the stack grows downward, then
    //we set all data for task-switching...
    *--stack = 0x0202;      // EFLAGS
    *--stack = 0x08;        // CS - next instruction
    *--stack = (unsigned int)run;// EIP - this points to a function, to control the status of the task

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
    strcpy(new_task->name, name);
    new_task->priority = priority;
    new_task->pid=pid++;
    new_task->state = READY; //ready to run
    add(new_task, tasks);

    asm volatile("sti"); //reenable interrupts...
}

#include <multitasking.h>

unsigned int pid=0;

extern struct page_dir* current_dir;
extern struct page_dir* kernel_dir;
extern int current_ring;

unsigned int switching_enabled=0;

void move_stack(void* initial_esp, void* kstack, unsigned int size)
{
    unsigned int i;
    for(i=(unsigned int)kstack; i >=(unsigned int)kstack-size; i-=PAGE_SIZE) //We get pages from memory to store the stack
        alloc_frame( get_page(i, 1, kernel_dir), 0, 1);
    
    unsigned int cr3_r;
    
    // We invalidate *all* addresses in the TLB, we could use invlpg
    // but it's not well supported on old processors
    asm volatile("mov %%cr3, %0":"=r"(cr3_r));
    asm volatile("mov %0, %%cr3"::"r"(cr3_r));
    
    unsigned int old_esp, old_ebp;
    asm volatile("mov %%esp, %0":"=r"(old_esp));
    asm volatile("mov %%ebp, %0":"=r"(old_ebp));
    
    unsigned int offset=(unsigned int)(kstack - initial_esp);
    unsigned int new_esp, new_ebp;
    new_esp=old_esp+offset;
    new_ebp=old_ebp+offset;
    
    memcpy((void*)new_esp, (void*)old_esp, (unsigned int)initial_esp-old_esp);
    
    for(i=(unsigned int)kstack; i > (unsigned int)kstack-size; i-=4)
    {
        unsigned int tmp=*(unsigned int*)i;
        if((old_esp<tmp) && (tmp<(unsigned int)initial_esp))
        {
            tmp+=offset;
            unsigned int *tmp2=(unsigned int*)i;
            *tmp2=tmp;
        }
    }
    asm volatile("mov %0, %%esp" : : "r" (new_esp));
    asm volatile("mov %0, %%ebp" : : "r" (new_ebp));
}

void init_tasking(unsigned int stack)
{
    tasks=make_list();
    // Move the stack to a known position
    move_stack((void*)stack, (void*)KERNEL_STACK, STACK_SIZE);
    
    add_schedule_function(schedule);
    set_switching_status(1);
}

unsigned int tick_count()
{
    return tick;
}

unsigned int get_pid()
{
    return tasks->current->value->pid;
}

stack_t schedule(stack_t context)
{
    tick++;
    disable_int(__FILE__, __LINE__);
    //if no tasks or in kernel mode, we execute all processes in user mode!
    if(tasks->size==0 || get_run_level()==KERNEL_RING || !get_switching_status())
    {
        enable_int();
        return context;
    }
    
    //kprintf("[%d] %d", tasks->current->value->pid, tasks->size);
    //kprintf("%d sched\n", k++);
    tasks->current->value->stack=context;
    if(tasks->current->value->state==RUNNING)
        tasks->current->value->state=READY;
    struct task* tmp;
    while((tmp=next(tasks))->state == ENDED)
    {
        fclose(tmp->stdin);
        fclose(tmp->stdout);
        remove(tmp, tasks);
    }   

    update_kstack(tasks->current->value->kernel_stack+STACK_SIZE);
    
    current_dir=tasks->current->value->directory;
    load_dir(tasks->current->value->directory);
    tasks->current->value->state=RUNNING;
    enable_int();
    return tasks->current->value->stack;
}

inline void exit(int status)
{
    disable_int(__FILE__, __LINE__);

    struct task* current=tasks->current->value;
    current->ret_value=status;
    current->state=ENDED;

    enable_int();
    
    for(;;); //now stop the process...it will be effectively deleted when the scheduler redo the context switch...
}

void wrap_exit()
{
    exit(0);
}

int add_task(char* name, void* run, int argc, char** argv, int is_elf, int is_driver)
{
    if(/*name==(char*)NULL ||*/ strcmp(name, "")==0 || run==(void*)NULL)
    {
        return -1;
    }
    
    disable_int(__FILE__, __LINE__); //disable interrupts..

    unsigned int* stack;
    struct task* new_task=(struct task*)kmalloc(sizeof(struct task));
    
    new_task->argc=argc;
    new_task->argv=argv;
    new_task->messages=make_queue();
    new_task->is_driver=is_driver;
    new_task->stdin=fopen("stdin");
    new_task->stdout=fopen("stdout");
   
    
    //if kernel..
    if(pid==0)
        stack=(unsigned int*)KERNEL_STACK-STACK_SIZE;
    else
    {
        stack =(unsigned int*)(kmalloc_a(STACK_SIZE)+STACK_SIZE);//allocates 4096bytes...
        new_task->original_stack=(stack_t)stack-STACK_SIZE;
        if(stack == (unsigned int*)NULL)
            kpanic("Error while creating space for the new stack", 0x0, __FILE__, __LINE__);
        
        //because of the structure of x86, the stack grows downward, then
        //we set all data for task-switching...
        *--stack = 0x0202;
        *--stack = 0x08;        // CS
        if(!is_elf)
                *--stack = (unsigned int)dummy;// EIP - this points to a function, to control the status of the task
        else
                *--stack = (unsigned int)dummy_elf;
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
    }
    
    new_task->stack = (unsigned int)stack; //refreshes the position of the stack
    if(is_elf)
        new_task->exec_image=run;
    else
        new_task->run = run;
    
    //setup a new dir-table-pages  
    //if we're the kernel...
    new_task->directory = clone_directory(current_dir);
    
    strcpy(new_task->name, name);
    new_task->pid=pid++;
    new_task->state = READY; //ready to run
    
    if(pid==0)
        new_task->kernel_stack=KERNEL_STACK-STACK_SIZE;
    else
        new_task->kernel_stack=kmalloc_a(STACK_SIZE);
    add(new_task, tasks);
    enable_int(); //reenable interrupts...
    return pid-1;
}

void dummy()
{
    // Here we are sure we're in the right context, because current it's referring to ourselves
    exit(tasks->current->value->run(tasks->current->value->argc, tasks->current->value->argv)); //Little tricky uh?    
}

void dummy_elf()
{
    disable_int(__FILE__, __LINE__);
        
    struct Elf32* img=load_ELF(tasks->current->value->exec_image);
    int is_drv=tasks->current->value->is_driver;
    enable_int();
    if(!is_drv)
    {
        struct message* msg;
        while(strcmp((msg=(struct message*)asyn_receive(-1))->payload, "GOON")!=0);
    }
    set_loaded();
    // Here we are sure we're in the right context, because current it's referring to ourselves
    exit(img->entry(tasks->current->value->argc, tasks->current->value->argv)); //Little tricky uh?  
}


void wait(unsigned int pid)
{
    struct task* nav=get_task_by_pid(pid);
    int a=2, b;
    disable_int(__FILE__, __LINE__);
    int state=nav->state;
    enable_int();
    while(state!=ENDED) // Just to do something!
    {
        memcpy(&a, &b, sizeof(int));
        disable_int(__FILE__, __LINE__);
        state=nav->state;
        enable_int();
    }
}

struct task* get_task_by_pid(unsigned int pid)
{
    struct node* nav=tasks->first_node;
    while(nav->value->pid!=pid && nav->next!=tasks->first_node)
        nav=nav->next;
    return nav->value;
}

struct task* get_task_by_name(char* name)
{
    struct node* nav=tasks->first_node;
    while(strcmp(nav->value->name, name)!=0 && nav->next!=tasks->first_node)
        nav=nav->next;
    return nav->value;
}

struct FILE* get_stdin()
{
    disable_int(__FILE__, __LINE__);
    struct FILE* in=tasks->current->value->stdin;
    enable_int();
    return in;
}
struct FILE* get_stdout()
{
    disable_int(__FILE__, __LINE__);
    struct FILE* out=tasks->current->value->stdout;
    enable_int();
    return out;
}


void set_task_state(unsigned int state, unsigned int pid)
{
    disable_int(__FILE__, __LINE__); //We're modifyng a task!
    struct task* tsk=get_task_by_pid(pid);
    tsk->state=state;
    enable_int();
}

int set_switching_status(unsigned int enable)
{
    switching_enabled=enable;
    return enable;
}

int get_switching_status()
{
    return switching_enabled;
}

struct FILE* get_fg_stdin()
{
    disable_int(__FILE__, __LINE__);
    struct node* nav=tasks->first_node;
    while(nav->value->is_fg==0 && nav->next!=tasks->first_node)
        nav=nav->next;
    if(nav->next==(struct node*)NULL)
        return (struct FILE*)NULL;
    //kprintf("found FG %s\n", nav->value->name);
    enable_int();
    return nav->value->stdin;
}

void set_fg_task(int pid)
{
    disable_int(__FILE__, __LINE__);
    struct node* nav=tasks->first_node;
    while(nav->next!=tasks->first_node)
    {
        nav->value->is_fg=0;
        nav=nav->next;
    }
    
    get_task_by_pid(pid)->is_fg=1;    
    enable_int();
}

int add_boot_task(char* filename, int argc, char** argv, int is_drv)
{
    void* app_ptr=(void*)kmalloc(get_file_size(filename));
    read(app_ptr, filename);
    if(is_elf(app_ptr))
    {
        int pid=add_task(filename, app_ptr, argc, argv, 1, is_drv);
        if(!is_drv)
        {
            set_fg_task(pid);
            /* Send GOON message! */
            struct message msg;
            strcpy(msg.payload, "GOON");
            asyn_send(pid, &msg, get_pid());
        }
        return pid;
    }
    return -1;
}

int add_user_task(char* filename, int argc, char** argv)
{
    void* app_ptr=(void*)kmalloc(get_file_size(filename));
    read(app_ptr, filename);
    if(is_elf(app_ptr))
    {
        int pid=add_task(filename, app_ptr, argc, argv, 1, 0);
        set_fg_task(pid);
        /* Send GOON message! */
        struct message msg;
        strcpy(msg.payload, "GOON");
        asyn_send(pid, &msg, get_pid());

        return pid;
    }
    return -1;
}

int get_fg_pid()
{
    int pid;
    disable_int(__FILE__, __LINE__);        
    struct node* nav=tasks->first_node;
    while(nav->value->is_fg==0 && nav->next!=tasks->first_node)
        nav=nav->next;
    if(nav->next==(struct node*)NULL)
        return -1;
    pid=nav->value->pid;
    enable_int();
    return pid;
}

int kill(int pid)
{
    disable_int(__FILE__, __LINE__);        
       
    struct task* current=get_task_by_pid(pid);
    if(current==(struct task*)NULL)
        return -1;
    current->ret_value=-1;
    current->state=ENDED;

    enable_int();
    return 0;
}
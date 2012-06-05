#include <drivers/driver.h>

unsigned int drivers_loaded=0;

unsigned int install_driver(void* mem_pos, char* name)
{
    return add_task(name, mem_pos, 0, (char**)NULL, 1, 1);
}

void drv_reg_handler(char* name, unsigned char int_no)
{
    struct task* tsk=get_task_by_name(name);
    drivers[int_no]=tsk;    
}

struct task* get_driver_int(unsigned char int_no)
{
    return drivers[int_no];
}

void send_irq_ack(unsigned int pid)
{
    struct message ack_msg;//=(struct message*)kmalloc(sizeof(struct message));    
    strcpy(ack_msg.payload, ACK_INT);
    asyn_send(pid, &ack_msg,-1);
    //kfree(ack_msg);
}

void set_loaded()
{
    disable_int(__FILE__, __LINE__);
    drivers_loaded=1;
    enable_int();
}

int get_loaded()
{
    disable_int(__FILE__, __LINE__);
    int val=drivers_loaded;
    enable_int();
    return val;
}
#include <ipc/ipc_messages.h>

static unsigned int msg_id=0;

int asyn_send(unsigned int dest, struct message* msg, unsigned int reply_to)
{
    /*static LOCK* l=(LOCK*)NULL;
    if(l==(LOCK*)NULL)
        l=init_lock();
    lock(l);*/
    set_switching_status(0);
    struct task* d_task=get_task_by_pid(dest);
    msg->ID=msg_id;
    msg->reply_id=reply_to;
    if(is_syscall())
        msg->sender_id=get_pid();
    else
        msg->sender_id=0;
    push(msg, d_task->messages);
    int copy=msg_id++;
    //unlock(l);
    set_switching_status(1);
    return copy;
}

struct message* asyn_receive(int req_id)
{
    // That's unefficient:    
    // Continue extracting and reinserting messages...

    disable_int(__FILE__, __LINE__);
    struct task* me=tasks->current->value;
    enable_int();
    int tries=0;
    struct message* f=(struct message*)NULL;
    if(req_id==-1)
        while((f=front(me->messages))==(struct message*)NULL && (tries++)<=DEFAULT_TRIES);
    else
    {
        disable_int(__FILE__, __LINE__);
        int condition=(front(me->messages)->reply_id!=req_id);
        int queue_len=me->messages->length;
        enable_int();
        while(condition && tries <= DEFAULT_TRIES*queue_len)
        {
            disable_int(__FILE__, __LINE__);
            tries++;
            struct message tmp;
            memcpy(&tmp, front(me->messages), sizeof(struct message));
            pop(me->messages);
            push(&tmp, me->messages);
            condition=front(me->messages)->reply_id!=req_id;
            f=front(me->messages);
            queue_len=me->messages->length;
            enable_int();
        }
    }
    
    if(tries> DEFAULT_TRIES || f==(struct message*)NULL)
        return (struct message*)NULL;

    disable_int(__FILE__, __LINE__);
    struct message* cpy=(struct message*)kmalloc(sizeof(struct message));

    memcpy(cpy, f, sizeof(struct message)); 

    //Removes the message from the queue
    while(front(me->messages)!=f)
    {
        struct message tmp;
        memcpy(&tmp, front(me->messages), sizeof(struct message));
        pop(me->messages);
        push(&tmp, me->messages);
    }
    pop(me->messages);
    enable_int();
    return cpy;
}

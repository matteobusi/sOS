#include <structures/linked_list.h>


struct list* make_list()
{
    struct list* new_lst=(struct list*)kmalloc(sizeof(struct list));
    new_lst->first_node=(struct node*)NULL;
    new_lst->size=0;
    new_lst->current=new_lst->first_node;
    return new_lst;
}

struct task* next(struct list* lst)
{
    lst->current=lst->current->next;
    return lst->current->value;
}

void add(struct task* tsk, struct list* lst)
{
    struct node* n=(struct node*)kmalloc(sizeof(struct node));
    n->value=tsk;

    if(lst->size==0)
    {
        n->next=n;
        lst->first_node=n;        
        lst->current=lst->first_node=n;
    }
    else
    {
        struct node* tmp=lst->first_node->next;
        while(tmp->next!=lst->first_node)
            tmp=tmp->next;
        n->next=tmp->next;
        tmp->next=n;
    }
    lst->size++;
}

void remove(struct task* tsk, struct list* lst)
{
    struct node* tmp=lst->first_node->next;
    struct node* prec=lst->first_node;
    while(tmp!=lst->first_node && tmp->value!=tsk)
    {
        prec=tmp;
        tmp=tmp->next;
    }
    
    prec->next=tmp->next;
    kfree((void*)tmp->value->kernel_stack, __FILE__, __LINE__);
    kfree((void*)tmp->value->original_stack, __FILE__, __LINE__);
    
    kfree(tmp->value, __FILE__, __LINE__);
    kfree(tmp, __FILE__, __LINE__);
    lst->size--;
}
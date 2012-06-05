#include <structures/queue.h>

V_TYPE* empty_element=(V_TYPE*)NULL;

struct queue* make_queue()
{
   struct queue* q=( struct queue*)kmalloc(sizeof(struct queue));
   if(q==( struct queue*)NULL)
       return ( struct queue*)NULL;
   q->first=q->last=( struct q_node*)NULL;
   q->length=0;
   return q;
}
V_TYPE* front(const struct queue* q)
{
    disable_int(__FILE__, __LINE__);
    if(q->length==0)
    {   
        enable_int();
        return (V_TYPE*)NULL;
    }        
    enable_int();
    return q->first->value;
}
void pop(struct queue* q)
{
    disable_int(__FILE__, __LINE__);
    if(q->length==0)
    {   
        enable_int();
        return;
    }
    
    struct q_node* new_first=q->first->next;
    
    kfree(q->first->value, __FILE__, __LINE__);// We allocated this in push by copying!
    kfree(q->first, __FILE__, __LINE__);
    q->length--;
    q->first=new_first;
    if(q->length==1)
        q->last=q->first;
    enable_int();    
}
void push(const V_TYPE* e, struct queue* q)
{
    disable_int(__FILE__, __LINE__);
    V_TYPE* tmp = (V_TYPE*)kmalloc(sizeof(V_TYPE));
    memcpy(tmp, e, sizeof(V_TYPE));
    struct q_node* new_node=(struct q_node*)kmalloc(sizeof(struct q_node));
    new_node->value=tmp;
    new_node->next=(struct q_node*)NULL;
 
    if(q->length==0)
        q->first=q->last=new_node;
    else
    {
        q->last->next=new_node;
        q->last=new_node;
    }
    q->length++;
    enable_int();
}

V_TYPE* get_empty_element()
{
    if(empty_element==(V_TYPE*)NULL)
    {
        empty_element=(V_TYPE*)kmalloc(sizeof(V_TYPE));
        memset(empty_element, 0, sizeof(V_TYPE));
    }
    return empty_element;
}
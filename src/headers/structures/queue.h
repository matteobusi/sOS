/* 
 * File:   queue.h
 * Author: caos
 *
 * Created on May 7, 2012, 4:12 PM
 */

#ifndef QUEUE_H
#define	QUEUE_H

#include <commons.h>
#include <memory.h>
#include <ipc/ipc_messages.h>

#define V_TYPE struct message

struct queue
{
    struct q_node* first;
    struct q_node* last;
    int length;    
};

struct q_node
{
    V_TYPE* value;
    struct q_node* next;
};

struct queue* make_queue();
V_TYPE* front(const struct queue* q);
void pop(struct queue* q);
void push(const V_TYPE* e, struct queue* q);

V_TYPE* get_empty_element();

#endif	/* QUEUE_H */


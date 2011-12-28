/* 
 * File:   linked_list.h
 * Author: caos
 *
 * Created on December 13, 2011, 8:08 AM
 */

#ifndef LINKED_LIST_H
#define	LINKED_LIST_H

#include <../utilities/commons.h>
#include <memory.h>

struct node
{
    struct task* value;
    struct node* next;
};

struct list
{
    struct node* first_node;
    struct node* current;
    int size;
};

struct list* make_list();

struct task* next(struct list*);

void add(struct task*, struct list*);

void remove(struct task*, struct list*);
#endif	/* LINKED_LIST_H */


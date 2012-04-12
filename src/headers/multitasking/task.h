/* 
 * File:   task.h
 * \brief In this file there are all utilities for multitasking in SoS
 */


#ifndef TASK_H
#define	TASK_H

#include <utilities.h>
#include <memory.h>
#include <clib.h>
#include <paging.h>

typedef unsigned int stack_t;

enum STATE { READY=0, RUNNING=1, ENDED=2};

typedef int (*run_t)(int argc, char **argv);

extern unsigned int tick;
struct list* tasks;

struct task
{
    unsigned int pid; //the unique ID for the current task..
    stack_t stack;
    enum STATE state; //All possible states, in the enum..
    char name[50]; //The name for the process...
    int priority; //an ascending value
    int ret_value;
    //FILE* res[]; 
    struct page_dir* directory;
    run_t run; //a function pointer to the real task in memory
    int argc;
    char **argv;
};

unsigned int tick_count();
unsigned int get_pid();

void init_tasking(unsigned int);
struct task* next_task(int p);
void exit(int status);
void wait(unsigned int time);
unsigned int size(int p);
stack_t schedule(stack_t context);
void add_task(char* name, int priority, run_t run, int argc, char** argv);

void dummy();

#endif	/* TASK_H */


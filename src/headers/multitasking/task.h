/* 
 * File:   task.h
 * \brief In this file there are all utilities for multitasking in SoS
 */


#ifndef TASK_H
#define	TASK_H

#include <../utilities.h>
#include <../memory.h>
#include <../clib.h>



#define STACK_SIZE 0x1000
enum STATE { READY=0, RUNNING=1, ENDED=2};
extern unsigned int tick;

struct task
{
    unsigned int pid; //the unique ID for the current task..
    unsigned int stack;
    enum STATE state; //All possible states, in the enum..
    char name[50]; //The name for the process...
    int priority; //an ascending value
    //FILE* res[]; 
    void (*run)(); //a function pointer to the real task in memory
};

unsigned int tick_count();

void init_tasking();
struct task* next_task(int p);
void kill_me();
void wait(unsigned int time);
unsigned int size(int p);
unsigned int schedule(unsigned int context);
void add_task(char* name, int priority, void (*run)());

#endif	/* TASK_H */


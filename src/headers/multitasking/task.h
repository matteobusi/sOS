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
#include <userland/ring.h>
#include <structures/queue.h>

#include <ipc/ipc_messages.h>

#include <fs/vfs.h>

typedef unsigned int stack_t;

#define STACK_SIZE 4096
#define KERNEL_STACK 0xF0000000

typedef char STATE;
#define READY 0
#define RUNNING 1
#define ENDED 2
#define BLOCKED 3

typedef int (*run_t)(int argc, char **argv);

extern unsigned int tick;
struct list* tasks;

struct task
{
    unsigned int pid; //the unique ID for the current task..
    stack_t stack;
    stack_t original_stack;
    stack_t kernel_stack;
    
    STATE state; //All possible states, in the enum..
    char name[50]; //The name for the process...
    int ret_value;
    struct page_dir* directory;
    run_t run; //a function pointer to the real task in memory
    int argc;
    char **argv;
    
    int is_driver;
    
    //the queue for messages..
    struct queue* messages;
    
    //These are stdin and stdout for the process
    struct FILE* stdin;
    struct FILE* stdout;
    
    void* exec_image;
    
    int is_fg;
};

unsigned int tick_count();
unsigned int get_pid();

void init_tasking(unsigned int);
struct task* next_task(int p);
void exit(int status);
unsigned int size(int p);
stack_t schedule(stack_t context);
int add_task(char* name, void* run, int argc, char** argv, int is_elf, int is_driver);
int add_user_task(char* filename, int argc, char** argv);

int add_boot_task(char* filename, int argc, char** argv, int is_drv);
void move_stack(void* initial_esp, void* kstack, unsigned int size);
void wait(unsigned int pid);
struct task* get_task_by_pid(unsigned int pid);
struct task* get_task_by_name(char* name);

struct FILE* get_stdin();
struct FILE* get_stdout();
struct FILE* get_fg_stdin();
int get_fg_pid();
void set_fg_task(int pid);
int kill(int pid);

void dummy();
void dummy_elf();
void wrap_exit();

void set_task_state(unsigned int state, unsigned int pid);

int set_switching_status(unsigned int enable);
int get_switching_status();

#endif	/* TASK_H */


/* 
 * File:   syscalls.h
 * Author: caos
 *
 * Created on April 12, 2012, 4:45 PM
 */

#ifndef SYSCALLS_H
#define	SYSCALLS_H

#include <interrupts/isr.h>

typedef int (*syscall_t)(int pid, ...);

void start_calls();
int add_call(syscall_t call);
void syscall_mngr(struct registers r);

#endif	/* SYSCALLS_H */


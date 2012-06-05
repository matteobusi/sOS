/* 
 * File:   syscalls.h
 * Author: caos
 *
 * Created on April 12, 2012, 4:45 PM
 */

#ifndef SYSCALLS_H
#define	SYSCALLS_H

#include <interrupts/isr.h>

#include <clib/stdout.h>

#include <memory.h>
#include <ipc/ipc_messages.h>

#include <drivers/driver.h>

#include <fs/vfs.h>

#include <clib/stdout.h>

#define MAX_SYSCALLS 256

typedef void* syscall_t;

int call_id;
syscall_t calls_vector[MAX_SYSCALLS];

void start_calls();
int add_call(syscall_t call);
void garbage_collect();
unsigned int syscall_mngr(struct registers* r);
int is_syscall();

#endif	/* SYSCALLS_H */


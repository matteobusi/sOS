/* 
 * File:   semaphore.h
 * Author: caos
 *
 * Created on May 13, 2012, 11:46 AM
 */

#ifndef MUTEX_H
#define	MUTEX_H

#include <memory.h>

typedef struct 
{ 
    int entries;
    int exits;
} LOCK;

LOCK* init_mutex();
void lock(LOCK* l);
void unlock(LOCK* l);
void end_lock(LOCK* d);

#endif	/* MUTEX_H */


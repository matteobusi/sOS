/* 
 * File:   newfile.h
 * Author: caos
 *
 * Created on September 20, 2010, 8:37 PM
 */

#ifndef HEAP_H
#define	HEAP_H

#include <../interrupts.h>
#include <../clib.h>
#include <../utilities.h>

#include <paging.h>

//Base and max addresses of the kernel heap - from HEAP_BASE to the end of memory
#define HEAP_BASE (0xC0000000 - sizeof(struct mem_block_header))
#define HEAP_INIT 28*1024*1024
#define HEAP_END (HEAP_BASE + HEAP_INIT)

// How much for user program's? 1/2 of the total memory for the heap
#define HEAP_USER_PROG (HEAP_INIT/2)
//Magic Heap Number
#define HEAP_MAGIC 0xDEADBEEF


struct mem_block_header
{
    struct mem_block_header* next;
    struct mem_block_header* previous;
    int magic;
    unsigned int size;
    unsigned char is_free;
    unsigned int phys;
};

//Init the heap module
void init_heap();

//function to dynamically allocate memory - use kmalloc
void* alloc(unsigned int size, char aligned);

//function to free allocated memory
//kpanic is generated if ptr is not valid, corrupted or not allocated by alloc
void kfree(void* ptr, char* f, int l);

#endif	/* HEAP_H */


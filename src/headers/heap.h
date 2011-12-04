/* 
 * File:   newfile.h
 * Author: caos
 *
 * Created on September 20, 2010, 8:37 PM
 */

#ifndef HEAP_H
#define	HEAP_H

//Base and max addresses of the kernel heap - 1GB default
#define HEAP_BASE 0xC0000000
#define HEAP_END  0xF0000000

//Magic Heap Number
#define HEAP_MAGIC 0xdeadbeef


struct mem_block_header{
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
void kfree(void* ptr);

#endif	/* HEAP_H */

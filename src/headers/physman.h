#ifndef PHYSMANAGER_INCLUDED_H
#define PHYSMANAGER_INCLUDED_H

#include "heap.h"

//Various allocators
unsigned int kmalloc_aligned_phys(unsigned int size, unsigned char aligned, unsigned int* phys);
unsigned int kmalloc(unsigned int size);
unsigned int kmalloc_a(unsigned int size);
unsigned int kmalloc_p(unsigned int size, unsigned int* phys);
unsigned int kmalloc_ap(unsigned int size, unsigned int* phys);
#endif // PHYSMANAGER_INCLUDED_H

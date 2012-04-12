#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

#include <boot.h>
#include <clib.h>

/*
 * These are functions user to simplify the work and to clear the kernel's main function
 */
void print_logo();
void memory_info(void* m_boot_addr);
void init_effect(char* text);

#endif // GRAPHICS_H_INCLUDED

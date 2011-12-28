#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

//FROM GNU Multiboot specification
#include <../boot.h>

#include <../clib.h>

//Prints VoxOS logo
void print_logo();
//Prints memeory Infos
void memory_info(void* m_boot_addr);
//Prints <text> ==========> [DONE] - usually used to show module's loading...
void init_effect(char* text);

#endif // GRAPHICS_H_INCLUDED

#ifndef GDT_H_INCLUDED
#define GDT_H_INCLUDED

#include <../clib.h>
#include <userland.h>

#define GDT_LEN 6

struct gdt_pointer {
    unsigned short limit;
    unsigned int base;

} __attribute__((packed));

struct gdt_entry {
    unsigned short  limit_low;
    unsigned short  base_low;
    unsigned char   base_middle;
    unsigned char   access;
    unsigned char   granularity;
    unsigned char   base_high;
} __attribute__((packed));

void gdt_set_gate(int index, unsigned int base, unsigned int limit, unsigned char access, unsigned char granularity);

//Inits GDT module
void init_gdt();

//Assembly function to load the gdt
extern void gdt_flush(unsigned int);

struct gdt_pointer gdtp;
struct gdt_entry gdt_table[GDT_LEN];
#endif // GDT_H_INCLUDED

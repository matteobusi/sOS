/* 
 * File:   tss.h
 * Author: caos
 *
 * Created on April 3, 2012, 9:38 PM
 */

#ifndef TSS_H
#define	TSS_H



#include <memory.h>

// We're going to define the TSS structure,
// please refer the IA-32 manual for specific informations about the TSS (Chapter 7, Paragraph 2)
// and check the right chapter in the SoS "book"

struct tss
{
    struct tss* prev_tss; // Link to the previous TSS in memory - according to the intel manual only 16 bit are used
    unsigned int esp0; // The stack pointer to use in kernel mode
    unsigned int ss0; // The stack segment for the kernel mode
    unsigned int esp1; // We're not going to use these fields
    unsigned int ss1;
    unsigned int esp2;
    unsigned int ss2;
    
    unsigned int cr3;
    unsigned int eip;
    unsigned int eflags;
    unsigned int eax;
    unsigned int ecx;
    unsigned int edx;
    unsigned int ebx;
    unsigned int esp;
    unsigned int ebp;
    unsigned int esi;
    unsigned int edi;
    
    // Now 16bit registers
    unsigned int es;
    unsigned int cs;
    unsigned int ss;
    unsigned int ds;
    unsigned int fs;
    unsigned int gs;
    unsigned int ldt;
    unsigned int trap; // Used only when hw task switching is in use
    unsigned int iomap_base;
} __attribute__((packed));


struct tss tss_entry;
extern void tss_flush();
void set_tss(int index, unsigned short ss0, unsigned int esp0);
void update_kstack(unsigned int stack);

#endif	/* TSS_H */


#include <userland/tss.h>

void set_tss(int index, unsigned short ss0, unsigned int esp0)
{
    unsigned int base=(unsigned int)&tss_entry;
    unsigned int limit=(unsigned int)base+sizeof(struct tss);
    
    // Setup the gate to host our TSS
    gdt_set_gate(index, base, limit, 0xE9, 0x00);
    
    // Now ensure the entry is completely set to 0
    memset(&tss_entry, 0, sizeof(struct tss));
    
    // Now setup the real TSS
    tss_entry.ss0=ss0;
    tss_entry.esp0=esp0;
    
    // Set cs=0x08 | 0x03=0xb
    tss_entry.cs=0xb;
    
    // Others = 0x10 | 0x03 = 0x13
    tss_entry.ss = tss_entry.ds = tss_entry.es = tss_entry.fs = tss_entry.gs = 0x13;
}

void update_kstack(unsigned int stack)
{
    tss_entry.esp0=stack;
}
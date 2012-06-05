#include <memory/gdt.h>

void gdt_set_gate(int index, unsigned int base, unsigned int limit, unsigned char access, unsigned char granularity)
{
    if(index < 0 || index >= GDT_LEN)
        return;

    //Limit
    gdt_table[index].limit_low = (limit & 0xFFFF);
    gdt_table[index].granularity = (limit >> 16) & 0x0F; //It's not the same as >> 28?

    //Base
    gdt_table[index].base_low = (base & 0xFFFF);
    gdt_table[index].base_middle = (base >>16) & 0xFF;
    gdt_table[index].base_high = (base >> 24) & 0xFF;

    //access and granularity
    gdt_table[index].access = access;
    gdt_table[index].granularity |= (granularity & 0xF0);
}

void init_gdt()
{
    //Sets up the gdt pointer
    gdtp.limit = (sizeof(struct gdt_entry)*GDT_LEN)-1;
    gdtp.base = (unsigned int)&gdt_table;

    /*the first NULL entry*/
    gdt_set_gate(0, 0,0,0,0);
    
    // 0x9A, 0x92, 0xFA, 0xF2
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);
    
    // Ok, add the tss
   
    set_tss(5, 0x10, 0x0);

    /*actually flushes gdt with lgdt code*/
    gdt_flush((unsigned int)&gdtp);
    
    // now flush the TSS
    tss_flush();
}

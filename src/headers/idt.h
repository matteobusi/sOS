#ifndef IDT_H_INCLUDED
#define IDT_H_INCLUDED


#define IDT_LEN 256

struct idt_entry{
    unsigned short base_low;
    unsigned short selector;
    unsigned char zero;
    unsigned char flags;
    unsigned short base_high;
} __attribute__((packed));

struct idt_pointer{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));
extern void idt_flush(unsigned int);

//function to set an idt entry
void idt_set_gate(int index, unsigned int base, unsigned short selector, unsigned char flags);
//inits the IDT module
void init_idt();

struct idt_pointer idt_p;
struct idt_entry idt_table[IDT_LEN];

#endif // IDT_H_INCLUDED

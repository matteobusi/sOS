
#include <idt.h>

#include <system.h>
#include <commons.h>
#include <isr.h>
#include <irq.h>

void idt_set_gate(int index, unsigned int base, unsigned short selector, unsigned char flags)
{
    idt_table[index].base_high = (base >> 16) & 0xFFFF;
    idt_table[index].base_low = base & 0xFFFF;

    idt_table[index].selector = selector;
    idt_table[index].zero = 0;

    idt_table[index].flags = flags;
}

void init_idt()
{
    idt_p.limit = (sizeof(struct idt_entry)*256)-1;
    idt_p.base = (unsigned int)&idt_table;

    memset(idt_table, 0, (sizeof(struct idt_entry)*256));

    remap_IRQ();
    map_isr();
    idt_flush((unsigned int)&idt_p);
}

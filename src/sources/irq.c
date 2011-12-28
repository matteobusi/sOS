#include <interrupts/irq.h>

void remap_IRQ()
{
    outportb(PIC1_COMMAND, ICW1_INIT+ICW1_ICW4);
    outportb(PIC2_COMMAND, ICW1_INIT+ICW1_ICW4);

    outportb(PIC1_DATA, 0x20); //0x20 = 32d is the address of the first free idt
    outportb(PIC2_DATA, 0x28); //0x28 = 40d is the address of the first free idt after the first PIC

    outportb(PIC1_DATA, 0x04);
    outportb(PIC2_DATA, 0x02);

    outportb(PIC1_DATA, ICW4_8086);
    outportb(PIC2_DATA, ICW4_8086);

    outportb(PIC1_DATA, 0x0);
    outportb(PIC2_DATA, 0x0);

    //then it sets the entries in IDT
    idt_set_gate(32, (unsigned int)irq0 , 0x08, 0x8E);
    idt_set_gate(33, (unsigned int)irq1 , 0x08, 0x8E);
    idt_set_gate(34, (unsigned int)irq2 , 0x08, 0x8E);
    idt_set_gate(35, (unsigned int)irq3 , 0x08, 0x8E);
    idt_set_gate(36, (unsigned int)irq4 , 0x08, 0x8E);
    idt_set_gate(37, (unsigned int)irq5 , 0x08, 0x8E);
    idt_set_gate(38, (unsigned int)irq6 , 0x08, 0x8E);
    idt_set_gate(39, (unsigned int)irq7 , 0x08, 0x8E);
    idt_set_gate(40, (unsigned int)irq8 , 0x08, 0x8E);
    idt_set_gate(41, (unsigned int)irq9 , 0x08, 0x8E);
    idt_set_gate(42, (unsigned int)irq10, 0x08, 0x8E);
    idt_set_gate(43, (unsigned int)irq11, 0x08, 0x8E);
    idt_set_gate(44, (unsigned int)irq12, 0x08, 0x8E);
    idt_set_gate(45, (unsigned int)irq13, 0x08, 0x8E);
    idt_set_gate(46, (unsigned int)irq14, 0x08, 0x8E);
    idt_set_gate(47, (unsigned int)irq15, 0x08, 0x8E);

    //
    memset(&handlers_list, 0, (sizeof(isr_t)*256));
}

void irq_handler(struct registers reg)
{
    //Sends EOI
    //if irq no >= 40 sends it also to the second PIC
    if(reg.int_no>=40)
        outportb(PIC2_COMMAND, 0x20);
    outportb(PIC1_COMMAND, 0x20);

    //Calls the callback setted for the current IRQ
    if(handlers_list[reg.int_no] != 0)
    {
        isr_t handler = handlers_list[reg.int_no];
        handler(reg);
    }
}

#ifndef ISR_H_INCLUDED
#define ISR_H_INCLUDED

#define ISR_LEN 32
#include <commons.h>
#include <idt.h>

typedef void (*isr_t)(struct registers reg);
isr_t handlers_list[IDT_LEN];


// These extern directives let us access the addresses of our ASM ISR handlers.
extern void isr0 ();
extern void isr1 ();
extern void isr2 ();
extern void isr3 ();
extern void isr4 ();
extern void isr5 ();
extern void isr6 ();
extern void isr7 ();
extern void isr8 ();
extern void isr9 ();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

//function to map all isr to the correspondent idt gate
extern void map_isr();
//isr exception handler
extern void exception_handler(struct registers reg);
//ISR handler adder
extern void add_handler(unsigned char n, isr_t handler);

#endif // ISR_H_INCLUDED

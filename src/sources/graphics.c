
#include <utilities/graphics.h>

void print_logo()
{
    char logo[][50]= {" ", "SOS - A multitaking simple hybrid kernel",
                      "Version 0.0.1"};
    int i;
    for(i=0; i < 3; i++)
        kprintf("%80s", logo[i]);    
}

void memory_info(void* m_boot_addr)
{
    extern unsigned int kend;
    multiboot_info_t* mboot_ptr = (multiboot_info_t*) m_boot_addr;
    //Shows where the kernel ends
    set_color(0, 9);
    kprintf("*** Kernel ends at: ");
    set_color(0, 7);
    kprintf("%d\n", &kend);
    //Shows informations about memory
    set_color(0, 9);
    kprintf("*** Lower Memory (KB): ");
    set_color(0, 7);
    kprintf("%d **", mboot_ptr->mem_lower);
    
    set_color(0, 9);
    kprintf(" Upper Memory (KB): ");
    set_color(0, 7);
    kprintf("%d ***\n", mboot_ptr->mem_upper);
}
void init_effect(char* text)
{
    int i;
    kprintf("%s ", text);
    for(i=0; i < 10; i++)
        kprintf("=");
    kprintf("> [");
    set_color(0, 9);
    kprintf("DONE");
    set_color(0, 7);
    kprintf("]\n");
}
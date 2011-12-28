
#include <utilities/graphics.h>

static void print_offset(int n)
{
    set_color(0,7);
    int i;
    for(i=0; i  < n; i++)
        kprintf(" ");
    set_color(7,0);
}

extern void print_logo()
{
    char logo[][50]= {" *** SoS * VoxOS * Step 2.1 ***\n",
                      "Building a basic multitasking OS based\n",
                      "on Round-Robin Scheduling with priorities\n",
                      "Released under GNU GPL2\n"};
    int i;
    for(i=0; i < 4; i++)
    {
        print_offset((COLUMNS-strlen(logo[i]))/2);
        kprintf(logo[i]);
    }
    set_color(0,7);
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
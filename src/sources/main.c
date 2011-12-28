/***
 *
 *                   ,----..                                                            .--,-``-.
 *                  /   /   \    .--.--.                  ,----..        ,----..      /   /     '.
 *                 /   .     :  /  /    '.               /   /   \      /   /   \    / ../        ;
 *                .   /   ;.  \|  :  /`. /              /   .     :    /   .     :   \ ``\  .`-    '
 *      .--.--.  .   ;   /  ` ;;  |  |--`       .---.  .   /   ;.  \  .   /   ;.  \   \___\/   \   :
 *     /  /    ' ;   |  ; \ ; ||  :  ;_        /.  ./| .   ;   /  ` ; .   ;   /  ` ;        \   :   |
 *    |  :  /`./ |   :  | ; | ' \  \    `.   .-' . ' | ;   |  ; \ ; | ;   |  ; \ ; |        /  /   /
 *    |  :  ;_   .   |  ' ' ' :  `----.   \ /___/ \: | |   :  | ; | ' |   :  | ; | '        \  \   \
 *     \  \    `.'   ;  \; /  |  __ \  \  | .   \  ' . .   |  ' ' ' : .   |  ' ' ' :    ___ /   :   |
 *      `----.   \\   \  ',  /  /  /`--'  /  \   \   ' '   ;  \; /  | '   ;  \; /  |   /   /\   /   :
 *     /  /`--'  / ;   :    /  '--'.     /    \   \    \   \  ',  /___\   \  ',  /___/ ,,/  ',-    .
 *    '--'.     /   \   \ .'     `--'---'      \   \ |  ;   :    //  .\;   :    //  .\ ''\        ;
 *      `--'---'     `---`                      '---"    \   \ .' \  ; |\   \ .' \  ; \   \     .'
 *                                                        `---`    `--"  `---`    `--" `--`-,,-'
 *
 */

#include <boot.h>
#include <clib.h>
#include <interrupts.h>
#include <memory.h>
#include <multitasking.h>
#include <utilities.h>

extern unsigned int init_address;
extern unsigned int kend;

multiboot_info_t* mboot_ptr;

void file_loader()
{
    int n=10, i;
    kprintf("Reading [config] file...\n");
    kprintf("\tLoading %d services...\n", n);
    for(i=0; i < n; i++)
        kprintf("\tLoading file %s [%d/%d]\n", "lol.txt", i+1, n);
    for(;;);
}

int kmain(unsigned int magic_number, void* m_boot_addr)
{
    mboot_ptr = (multiboot_info_t*) m_boot_addr;

    //Init GDT, IDT and Video
    init_video();
    //Clears the screen
    clear();
    init_effect("Video");
    print_logo();
    init_gdt();
    init_effect("Global Descriptor Table");
    init_idt();
    init_effect("Interrupt Descriptor Table");
    
    if(magic_number != MULTIBOOT_BOOTLOADER_MAGIC)
        kpanic("I'm a multiboot-compliant OS and I need a multiboot BootLoader! :(", 0x0,__FILE__, __LINE__);
    
    //init the timer with a frequecy of 5Hz
    init_timer(200);
    init_effect("Timer");
    //enables paging...
    init_address = (unsigned int)&kend;
    init_paging(mboot_ptr->mem_upper * 1024 - (unsigned int )&kend);

    init_effect("Paging");

    //creates the heap!
    init_heap();
    init_effect("Heap");
    kprintf("\n");
    memory_info(mboot_ptr);
    
    init_tasking();
    init_effect("Kernel Threads");
    
    add_task("kernel", 0, kmain);
    init_effect("\tkernel");        
    //Enables interrupts...
    asm volatile("sti");

    add_task("loader", 0, file_loader);
    init_effect("\tserver_loader");
    
    for(;;);//kprintf("I'm the kernel!\n");
    return 0;
}
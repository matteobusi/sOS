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

#include <system.h>
#include <stdout.h>

#include <pit.h>
#include <gdt.h>
#include <idt.h>

#include <graphics.h>

#include <paging.h>
#include <physman.h>
#include <heap.h>

#include <panic.h>

#include <initrd.h>
#include <commons.h>

//FROM GNU Multiboot specification
#include <multiboot.h>

extern unsigned int init_address;
extern unsigned int kend;

int kmain(unsigned int magic_number, void* m_boot_addr)
{
    multiboot_info_t* mboot_ptr = (multiboot_info_t*) m_boot_addr;

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
        kpanic("I'm a multiboot-compliant OS and I need a multiboot BootLoader! :(", 0x0, __FILE__, __LINE__);
    
    //Enables the interrupts
    asm volatile("sti");
    //init the timer with a frequecy of 100Hz
    init_timer(100);
    init_effect("Timer");

    //enables paging...
    init_address = (unsigned int)&kend;
    init_paging(mboot_ptr->mem_upper * 1024 - (unsigned int )&kend);
    
    init_effect("Paging");
   

    //creates the heap!
    init_heap();
    init_effect("Heap");
    kprintf("\n");
    memory_info(m_boot_addr);
    int i;
    for(i=0; i < 5; i++)
    {
        unsigned int addr=-1;
        char* tmp=(char*)kmalloc_aligned_phys(sizeof(char)*10, 1, &addr);
        kprintf("I've allocated 10 char aligned:\n@: %h\n@Phys: %h\nAligned: %d\n", tmp, addr, 1-((int)tmp)%PAGE_SIZE);
        //kfree(tmp);
    }
    for(;;);
}
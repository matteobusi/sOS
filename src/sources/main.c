/*
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

#include <userland/ring.h>

extern unsigned int init_address;
extern unsigned int kend;

multiboot_info_t* mboot_ptr;

int kmain(unsigned int magic_number, void* m_boot_addr, unsigned int initial_esp) {
    mboot_ptr = (multiboot_info_t*) m_boot_addr;
        
    //Init GDT, IDT and Video
    init_video();
    //Clears the screen
    clear();
    print_logo();
    init_gdt();
    init_effect("Global Descriptor Table");
    init_idt();
    init_effect("Interrupt Descriptor Table");
    
    if (magic_number != MULTIBOOT_BOOTLOADER_MAGIC)
        kpanic("I'm a multiboot-compliant OS and I need a multiboot BootLoader! :(", 0x0, __FILE__, __LINE__);

    //init the timer with a frequecy of 200Hz
    init_timer(100);
    init_effect("Timer");
    //enables paging...

    unsigned int initrd_end = ((multiboot_module_t*) mboot_ptr->mods_addr)->mod_end;
    //since we have to load the initrd, we should move ahead the init address
    if ((mboot_ptr->flags & MULTIBOOT_INFO_MODS) && mboot_ptr->mods_count > 0)
        init_address = initrd_end;
    else
        init_address = (unsigned int) &kend;

    init_paging(mboot_ptr->mem_upper * 1024 - init_address);

    init_effect("Paging");

    //heap created with paging, just show we created it...
    init_effect("Heap");
    kprintf("\n");
    memory_info(mboot_ptr);

    init_tasking(initial_esp);
    asm volatile("sti");
    add_task("kernel", 0, (run_t)kmain, 0, (char**)NULL);
    
    init_effect("Multitasking");
    
    init_effect("SysCalls setup");
    start_calls();
    
    init_effect("User mode");
    switch_to_user();    
    
    asm volatile("int $128");
    
    
    for(;;);
    return 0;
}
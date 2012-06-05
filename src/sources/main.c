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

#include <structures/queue.h>

#include <userland/unistd.h>

#include <userland/ring.h>
#include <drivers/driver.h>

#include <vesa.h>

extern unsigned int init_address;
extern unsigned int kend;

multiboot_info_t* mboot_ptr;

extern struct page_dir* current_dir;

#define INIT_SRV_NAME "init"
#define STARTUP_CFG_FILE "init.cfg"

int kmain(unsigned int magic_number, void* m_boot_addr, unsigned int initial_esp) {
    mboot_ptr = (multiboot_info_t*) m_boot_addr;
    /* Prepares everything for the REAL kernel */
    init_video(mboot_ptr->vbe_mode, (vesa_mode_info_t*)mboot_ptr->vbe_mode_info);
    
    init_vesa((vesa_mode_info_t*)mboot_ptr->vbe_mode_info);
    
    /* So let's clear the screen */    
    clear();
    /* Show out logo */
    print_logo();
    /* And start mad memory thing... */
    init_gdt();
    init_effect("Global Descriptor Table");
    init_idt();
    init_effect("Interrupt Descriptor Table");

    if (magic_number != MULTIBOOT_BOOTLOADER_MAGIC)
        kpanic("I'm a multiboot-compliant OS and I need a multiboot BootLoader! :(", 0x0, __FILE__, __LINE__);
    
    /* Install the timer and tell the user */
    init_timer(200);
    init_effect("Timer");
    
    /* Now, get the module and do something.. */
    unsigned int initrd_end = ((multiboot_module_t*) mboot_ptr->mods_addr)->mod_end;
    /* If a module is found, please identity map it, so include in the "kernel space" */
    if ((mboot_ptr->flags & MULTIBOOT_INFO_MODS) && mboot_ptr->mods_count > 0)
        init_address = initrd_end;
    else
    {
         /* We cannot continue! So sorry */
        kpanic("No Initial RAM disk found...\n", 0x0, __FILE__, __LINE__);       
    }
    
    /* Setup paging and Heap together */

    init_paging(mboot_ptr->mem_upper * 1024);
    
    init_effect("Paging");
    init_effect("Heap");
    kprintf("\n");

    /* Show informations about memory */
    memory_info(mboot_ptr);
    void* module=(void*)(((multiboot_module_t*) mboot_ptr->mods_addr)->mod_start);
    
    if(!is_initrd(module))
        kpanic("Initial RAM disk found, but incorrect format!\nBoot ended: your system is useless without a RAM disk :(\n", 0x0, __FILE__, __LINE__);     
    
    init_tasking(initial_esp);
    add_task("kernel", (run_t)kmain, 0, (char**)NULL, 0, 0);
    init_effect("Multitasking");
    init_effect("SysCalls setup");
    start_calls();
    
    init_initrd(module);
    
    /* We've to take out the only "fixed" server, if it doesn't exist simply kill the system! */
    int init_srv_sz=get_file_size(INIT_SRV_NAME);
    if(init_srv_sz==-1) /* Oops file not found! */
        kpanic("Oops file not found, check your Initial RAM disk :(\n", 0x0, __FILE__, __LINE__);
    void* init_server_exc=(void*)kmalloc(init_srv_sz);
    read(init_server_exc, INIT_SRV_NAME);
    struct Elf32* init_server_elf = load_ELF(init_server_exc);
    if(last_err()!=NO_ERR)
         kpanic("Oops couldn't load initial server, check your Initial RAM disk :(\n", 0x0, __FILE__, __LINE__);
    
    run_t init_server_ep =  init_server_elf->entry;
    char* init_server_args[]={STARTUP_CFG_FILE};
    add_task(INIT_SRV_NAME, init_server_ep, 1, init_server_args, 0,0);
    init_effect("initial server");
    kprintf("Switching to ring3...\n");
    enable_int();
    switch_to_user(); 
   
    for(;;);
    return 0;
}

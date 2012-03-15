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

extern unsigned int init_address;
extern unsigned int kend;

multiboot_info_t* mboot_ptr;

int file_loader(int argc, char** argv) {
    
    kprintf("%d arguments:\n", argc);
    int i;
    for(i=0; i < argc; i++)
        kprintf("\targv[%d]:%s\n", i, argv[i]);
    
    int start = ((multiboot_module_t*) mboot_ptr->mods_addr)->mod_start;
    int size = (((multiboot_module_t*) mboot_ptr->mods_addr)->mod_end)-(((multiboot_module_t*) mboot_ptr->mods_addr)->mod_start);

    void* dest = (void*)kmalloc(size);
    memcpy(dest, (int*) start, size);
    
    run_t entry=load_ELF(dest);
    
    if(entry==(run_t)NULL && last_err()!=NO_ERR)
        kprintf("Error while loading file: %d\n", last_err());
    else
    {
        char name[50]="loaded_elf0";
        add_task(name, 0, entry, 0, (char**)NULL);
    }

    //kfree(dest);
    
    //load the elf file... it's a module!

    /*
    if(!(mboot_ptr->flags & MULTIBOOT_INFO_MODS))
    {
        kprintf("No modules to load...");
        exit(-1);
    }
    
    kprintf("Trying to load the InitRD:\n");

    unsigned int initrd_location = ((multiboot_module_t*)mboot_ptr->mods_addr)->mod_start;

          
    //the first module *must* be the initrd
    
    struct initrd_header fs_header;

    //initialized in init_initrd - it has to be freed up after being used
    struct initrd_file_header* fs_files;
    //
    if(init_initrd((void*)initrd_location, &fs_header, &fs_files)==-1)
    {
        kprintf("Error while loading initRD module: invalid magic number\n");
        exit(-1);
    }
    
    char** files_list=(char**)kmalloc(sizeof(char*)*fs_header.n_files);
    
    int n=list(files_list, fs_files, &fs_header);
    int i=0;
    //for(i=0; i < n; i++)
    {
        void* content=read(files_list[i], fs_files, &fs_header, (unsigned char*)initrd_location);
        run_t ep=content;
        char* par[]={"a", "b"};
        ep(3, par);
        //add_task("task1", 0, ep);
    }

    //frees resources
    kfree(fs_files);
     */
    return 0;
}

int kmain(unsigned int magic_number, void* m_boot_addr) {
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
    //since we have to load the initrd, we should move ahead this address
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

    init_tasking();
    init_effect("Multitasking");
    
    //Enables interrupts...
    asm volatile("sti");
    add_task("kernel", 0, (run_t) kmain, 0, (char**)NULL); //Adding the kernel task, everything starts here :)
    init_effect("\tkernel");

    char* par[3]={"Uno", "Due", "Tre"};
    add_task("file_loader", 0, file_loader, 3, par);
    init_effect("\tfile_loader");
        
    for(;;);
    return 0;
}
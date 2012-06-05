#include <../headers/executables/elf.h>
#include <../headers/clib.h>

static unsigned int err=NO_ERR;

int is_elf(void* mapped_file)
{
    struct Elf32_Ehdr* elf_header=mapped_file;
    if(elf_header->e_ident[EI_MAG0_POS]!=EI_MAG0 || elf_header->e_ident[EI_MAG1_POS]!=EI_MAG1 || elf_header->e_ident[EI_MAG2_POS]!=EI_MAG2 || elf_header->e_ident[EI_MAG3_POS]!=EI_MAG3)
        return 0;
    return 1;
}

struct Elf32* load_ELF(void* mapped_exec)
{
    err=NO_ERR;
    int i;
    
    struct Elf32* elf=(struct Elf32*)kmalloc(sizeof(struct Elf32));
    elf->elf_header = mapped_exec;
    
    //We check the magic number
    if(!is_elf(mapped_exec))
    {
        err=MAGIC_ERR;
        return (struct Elf32*)NULL;
    }
    
    //Here the platform    
    if(elf->elf_header->e_ident[EI_CLASS]!=CURRENT_CLASS || elf->elf_header->e_ident[EI_DATA]!=CURRENT_DATA || elf->elf_header->e_machine!=CURRENT_MAC)
    {
        err=ARCH_ERR;
        return (struct Elf32*)NULL;
    }
    
    //Here the version
    if(elf->elf_header->e_ident[EI_VERSION]!=EV_CURRENT || elf->elf_header->e_ident[EI_VERSION]!=elf->elf_header->e_version)
    {
        err=VER_ERR;
        return (struct Elf32*)NULL;
    }
    elf->program_image=mapped_exec;
        
    for(i=0; i < elf->elf_header->e_phnum; i++)
    {
        elf->program_headers[i]=(struct Elf32_Phdr*)(mapped_exec+elf->elf_header->e_phoff+i*elf->elf_header->e_phentsize); 

        if(elf->program_headers[i]->p_type==PT_LOAD)
        {
            unsigned int dest, src;
            unsigned int len;
            dest=elf->program_headers[i]->p_vaddr;
            // map(dest, map_to, get_task_by_pid(get_pid())->directory);
            len=elf->program_headers[i]->p_filesz;
            src=(unsigned int)mapped_exec+elf->program_headers[i]->p_offset;
            memcpy((void*)dest, (void*)src, len);
            dest+=len;
            memset((void*)dest, 0, elf->program_headers[i]->p_memsz-len);
        }
    }
    
    elf->entry=(run_t)elf->elf_header->e_entry;
    return elf;
}

int last_err()
{
    return err;
}

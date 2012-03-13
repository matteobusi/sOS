#include <../headers/executables/elf.h>
#include <../headers/clib.h>

static unsigned int err=NO_ERR;

run_t load_ELF(void* mapped_exec)
{
    int i;
    
    struct Elf32* elf=(struct Elf32*)kmalloc(sizeof(struct Elf32));
    elf->elf_header = (struct Elf32_Ehdr*)kmalloc(sizeof(struct Elf32_Ehdr));    
    memcpy(elf->elf_header, mapped_exec, sizeof(struct Elf32_Ehdr));
    
    //We check the magic number
    if(elf->elf_header->e_ident[EI_MAG0_POS]!=EI_MAG0 || elf->elf_header->e_ident[EI_MAG1_POS]!=EI_MAG1 || elf->elf_header->e_ident[EI_MAG2_POS]!=EI_MAG2 || elf->elf_header->e_ident[EI_MAG3_POS]!=EI_MAG3)
    {
        err=MAGIC_ERR;
        return (run_t)NULL;
    }
    
    //Here the platform
    if(elf->elf_header->e_ident[EI_CLASS]!=CURRENT_CLASS || elf->elf_header->e_ident[EI_DATA]!=CURRENT_DATA || elf->elf_header->e_machine!=CURRENT_MAC)
    {
        err=ARCH_ERR;
        return (run_t)NULL;
    }
    
    //Here the version
    if(elf->elf_header->e_ident[EI_VERSION]!=EV_CURRENT || elf->elf_header->e_ident[EI_VERSION]!=elf->elf_header->e_version)
    {
        err=VER_ERR;
        return (run_t)NULL;
    }
    
    for(i=0; i < elf->elf_header->e_phnum; i++)
    { 
        elf->program_headers[i]=(struct Elf32_Phdr*) kmalloc(sizeof(struct Elf32_Phdr));   
        if(elf->program_headers[i]==(struct Elf32_Phdr*)NULL)
        {
                err=MEM_ERR;
                return (run_t)NULL;
        }

        memcpy(elf->program_headers[i], mapped_exec+elf->elf_header->e_phoff+i*elf->elf_header->e_phentsize, elf->elf_header->e_phentsize);
    }
    
    
    unsigned int size=elf->program_headers[0]->p_memsz;
    elf->program_image=(void*)kmalloc(size);
    if(elf->program_image==NULL)
    {
        err=MEM_ERR;
        return (run_t)NULL;
    }
    
     for(i=0; i < elf->elf_header->e_phnum; i++)
            memcpy(elf->program_image+elf->program_headers[i]->p_offset, mapped_exec+elf->program_headers[i]->p_offset, elf->program_headers[i]->p_memsz);
    
    return (run_t)(elf->program_image);
}

int last_err()
{
    return err;
}

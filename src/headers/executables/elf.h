/* 
 * File:   elf.h
 * Author: caos
 * TODO: Load data section to allow global and static variables!
 * Created on February 25, 2012, 10:26 AM
 */

#ifndef ELF_H
#define	ELF_H

#include <multitasking.h>

/* Errors */
#define NO_ERR 0
#define MAGIC_ERR 1
#define ARCH_ERR 2
#define VER_ERR 3
#define MEM_ERR 4

/*
 * In this code we refer to paper at http://flint.cs.yale.edu/cs422/doc/ELF_Format.pdf and we define data type as defined at
 * page 8.
 */
typedef unsigned int Elf32_Addr;
typedef unsigned short Elf32_Half;
typedef unsigned int Elf32_Off;
typedef int Elf32_Sword;
typedef unsigned int Elf32_Word;

/*
 * defines for the ELF type, they define the type of the executable we have
 */
#define ET_NONE 0
#define ET_REL 1
#define ET_EXEC 2
#define ET_DYN 3
#define ET_CORE 4
#define ET_LOPROC 0xff00
#define ET_HIPROC 0xffff

/*
 * defines to specify the target machine for the ELF file
 */
#define EM_NONE 0
#define EM_M32 1
#define EM_SPARC 2
#define EM_386 3
#define EM_68K 4
#define EM_88K 5
#define EM_860 7
#define EM_MIPS 8

/*
 * Version of the ELF:
 *      - 0 specify an invalid version of the format
 *      - 1 this version (current)
 *      - >1 future versions
 */

#define EV_NONE 0
#define EV_CURRENT 1

/*
 * Now various define to specify the basic informations contained in the first 16bytes of the ELF
 */

// Position from 0 to 3
#define EI_MAG0_POS 0
#define EI_MAG0 0x7f
#define EI_MAG1_POS 1
#define EI_MAG1 'E'
#define EI_MAG2_POS 2
#define EI_MAG2 'L'
#define EI_MAG3_POS 3
#define EI_MAG3 'F'

// Position 4, defines the file class or capacity
#define EI_CLASS 4
// Possible values
#define ELFCLASSNONE 0
#define ELFCLASS32 1
#define ELFCLASS64 2

// Now data encoding specific for the processor-specific data in the object file
#define EI_DATA 5
// Possible values for the EI_DATA byte - specific informations about this data rapresentation are in the ELF pdf in bibliografy
#define ELFDATANONE 0
#define ELFDATA2LSB 1
#define ELFDATA2MSB 2

// This byte defines the version of the ELF file, must be EV_CURRENT, just like the field e_version of the header
#define EI_VERSION 6

// This byte is a padding information about how many bytes to ignore in the e_ident field of the header
#define EI_PAD 7

// This specifies the size of the e_ident field, it's fixed to 16
#define EI_NIDENT 16


// Defines this machine, for future portability
#define CURRENT_MAC EM_386
#define CURRENT_CLASS ELFCLASS32
#define CURRENT_DATA ELFDATA2LSB

/* This structure defines the header for the ELF file, we need it to store informations
 * about our binary file.
 * 
 * Please refer to the PDF in bibliography for specific informations.
 */
struct Elf32_Ehdr
{
    unsigned char e_ident[EI_NIDENT]; 
    Elf32_Half e_type;
    Elf32_Half e_machine;
    Elf32_Word e_version;
    Elf32_Addr e_entry;
    Elf32_Off e_phoff;
    Elf32_Off e_shoff;
    Elf32_Word e_flags;
    Elf32_Half e_ehsize;
    Elf32_Half e_phentsize;
    Elf32_Half e_phnum;
    Elf32_Half e_shentsize;
    Elf32_Half e_shnum;
    Elf32_Half e_shstrndx;
};

/*
 * Defines to specify possible values for p_type in Elf32_Phdr.
 * Please refer to page 33 of the specification to understand the meaning of each value.
 */
#define PT_NULL 0
#define PT_LOAD 1
#define PT_DYNAMIC 2
#define PT_INTERP 3
#define PT_NOTE 4
#define PT_SHLIB 5
#define PT_PHDR 6
#define PT_LOPROC 0x70000000
#define PT_HIPROC 0x7fffffff

/*
 * These are bit masks for the p_flags field in Elf32_Phdr
 */
#define PF_X 1
#define PF_W 2
#define PF_R 4

/*
 * This structure contains information for program header(s) in the elf file, position is given by the 
 * e_phoff field in ELF header.
 */

struct Elf32_Phdr
{
    Elf32_Word p_type;
    Elf32_Off p_offset;
    Elf32_Addr p_vaddr;
    Elf32_Addr p_paddr;
    Elf32_Word p_filesz;
    Elf32_Word p_memsz;
    Elf32_Word p_flags;
    Elf32_Word p_align;
};

struct Elf32_Shdr
{
   Elf32_Word sh_name; 		/* section name */
   Elf32_Word sh_type; 		/* SHT_... */
   Elf32_Word sh_flags; 	/* SHF_... */
   Elf32_Addr sh_addr; 		/* virtual address */
   Elf32_Off sh_offset; 	/* file offset */
   Elf32_Word sh_size; 		/* section size */
   Elf32_Word sh_link; 		/* misc info */
   Elf32_Word sh_info; 		/* misc info */
   Elf32_Word sh_addralign; 	/* memory alignment */
   Elf32_Word sh_entsize; 	/* entry size if table */
};

/*
 * This structure is not defined in the specification for the ELF file, but it's a way to keep informations for
 * elf files all togheter
 */

#define MAX_PROGRAM_HEADERS 3

struct Elf32
{    
    void* program_image;
    run_t entry;
    struct Elf32_Ehdr* elf_header;
    struct Elf32_Phdr* program_headers[MAX_PROGRAM_HEADERS];
};

/*
 * This function has been built to load ELF executables, mainly for kernel modules (and server) and drivers.
 * It returns a pointer to the entry point of the file, usually used to create the process.
 * On error returns NULL and set the flag err to a specific value, defined by defines in this file.
 * err value can be retrieved by using int last_err()
 * 
 * You can FREE mapped_exec after loading!
 */
struct Elf32* load_ELF(void* mapped_exec);

/*
 * This function returns the last error done by the loadELF functions, 
 * possible error codes are specified in this file by *_ERR defines
 */
int last_err();

int is_elf(void* mapped_file);
#endif	/* ELF_H */


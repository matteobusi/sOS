/**
 * Initial RAM disk header, used to load configuration files and programs at the run-time
 * 
 * \author Matteo Busi
 */

#ifndef INITRD_H
#define	INITRD_H

#include <memory.h>
#include <interrupts.h>
#include <utilities.h>
#include <clib.h>

#define MAX_INITRD_FILES 255
#define MAX_FILE_NAME_LEN 256

/**
 * InitRD header, this header is intended to be the "base" for all the initrd structure
 */
struct initrd_header
{
    int magic; /**< The magic number for the initrd */
    int n_files; /**< Number of files contained int the initRD */
    int file_section_offset; /**< Where is the offset of the file section */
};

/**
 * InitRD file heaer, each file belonging to the initrd is identified by this header
 */
struct initrd_file_header
{
    char filename[MAX_FILE_NAME_LEN]; /**< The filename for the file */
    int size;/**< Size of the file in bytes */
    int offset; /**< Offset from the start of the file section */
};

int is_initrd(void* memory_mapped_file);
int init_initrd(void* memory_mapped_file);
int list(char vector[][MAX_FILE_NAME_LEN]);
void read(void* dest_buffer, const char* filename);
unsigned int count_files();
char* get_file_name_at(int index);
int get_file_size(const char* filename);

#endif	/* INITRD_H */


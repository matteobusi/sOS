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
    char filename[256]; /**< The filename for the file */
    int size;/**< Size of the file in bytes */
    int offset; /**< Offset from the start of the file section */
};


int is_initrd(void* memory_mapped_file);

/**
 * Function to initialize InitRD module of the kernel
 * \return 0 success, other error
 * @param[in] memory_mapped_file The pointer to the initrd in memory
 * @param[out] fs_header Pointer to a chunck of memory with size uqual to sizeof(struct initrd_header)
 * @param[out] fs_files Vector of pointers to initrd_file_header allocated dynamically by the function, will be filled with headers for all files
 */
int init_initrd(void* memory_mapped_file, struct initrd_header* fs_header, struct initrd_file_header** fs_files);

/**
 * Fills the initrd_header structure passed, given the initrd file
 * @param[out] header Pointer to the chunk of memory where to fill with 
 */
void fill_header(struct initrd_header* header, unsigned char* file);

/**
 * Fills the initrd_file_header structure passed, given the initrd file and the header
 */
void fill_files(struct initrd_file_header* files, const struct initrd_header* header,unsigned char* file);

/**
 * Read the content of a file
 * 
 * @param file_name Name of the file to read
 * @param files Files' header
 * @param header Header of the InitRD
 * @param file_datas The pointer to in-memory file of the InitRD
 * @return Returns a dynamic allocated pointer to a chunk of memory containing the file
 */
char* read(const char* file_name, const struct initrd_file_header* files, const struct initrd_header* header, const unsigned char* file_datas);

/**
 * List files in the InitRD: fills the vector with pointers to file names and return their number 
 * 
 * @param[out] vector The vector filled with file names
 * @param[in] files Files' header
 * @param[in] header Header of the InitRD
 * @return The number of files in the InitRD
 */
int list(char** vector, const struct initrd_file_header* files, const struct initrd_header* header);

#endif	/* INITRD_H */


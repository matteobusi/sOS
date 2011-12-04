/* 
 * File:   initrd.h
 * Author: caos
 *
 * Created on December 6, 2010, 6:11 PM
 */

#ifndef INITRD_H
#define	INITRD_H

struct initrd_header
{
    int magic;
    int n_files;
    int file_section_offset;
};

struct initrd_file_header
{
    char filename[256];
    int size;
    int offset;
};

//Inits initrd module
//Returns 0 if success
//-1 if unsuccessful
int init_initrd(void* memory_mapped_file, struct initrd_header* fs_header, struct initrd_file_header** fs_files);

//Fills the initrd_header structure passed, given the initrd file
void fill_header(struct initrd_header* header, unsigned char* file);

//Fills the initrd_file_header structure passed, given the initrd file and the header
void fill_files(struct initrd_file_header* files, const struct initrd_header* header,unsigned char* file);

//file_name -> file to read
//files -> files' headers
//header -> initrd header
//datas -> files contents
//**Returns a dynamic allocated ptr to a chunck of memory containing the file**
char* read(const char* file_name, const struct initrd_file_header* files, const struct initrd_header* header, const unsigned char* file_datas);

//fill the vector with pts to file names and return their number
int list(char** vector, const struct initrd_file_header* files, const struct initrd_header* header);

#endif	/* INITRD_H */


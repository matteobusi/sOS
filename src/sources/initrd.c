#include <commons.h>
#include <system.h>
#include <physman.h>
#include <initrd.h>
#include <panic.h>

#include <stdout.h>

#define MAGIC 0x425553

//Builds a fs header and a file_header array
struct initrd_header fs_header;
struct initrd_file_header* fs_files;

int init_initrd(void* memory_mapped_file, struct initrd_header* fs_header, struct initrd_file_header** fs_files)
{
   unsigned char* file = (unsigned char*) memory_mapped_file;
   fill_header(fs_header, file);
   if(fs_header->magic != MAGIC)
       return -1;
   
   //allocates memory for files...
   *fs_files =(struct initrd_file_header*) kmalloc(sizeof(struct initrd_file_header)*(fs_header->n_files));

   //fill them...
   fill_files(*fs_files, fs_header, file);
   return 0;
}

void fill_header(struct initrd_header* header, unsigned char* file)
{
    memcpy(header, file, sizeof(struct initrd_header));
}

void fill_files(struct initrd_file_header* files, const struct initrd_header* header,unsigned char* file)
{
    //Copies in the files' header all files' haeaders
    //They starts at the end of the inird header, so at sizeof(struct initrd_header)
    memcpy(files, &file[sizeof(struct initrd_header)], sizeof(struct initrd_file_header)*header->n_files);
}

//fill the vector with pts to file names and return their number
int list(char** vector, const struct initrd_file_header* files, const struct initrd_header* header)
{
    int i;
    for(i=0; i < header->n_files; i++)
        vector[i] = (char*)files[i].filename;
    return header->n_files;
}

//file_name -> file to read
//files -> files' headers
//header -> initrd header
//datas -> files contents

//Returns a dynamic allocated ptr to a chunck of memory containing the file
char* read(const char* file_name, const struct initrd_file_header* files, const struct initrd_header* header, const unsigned char* file_datas)
{
    int i;
    for(i=0; i < header->n_files; i++)
      if(strcmp(file_name, files[i].filename)==0)
            break;
    if(i == header->n_files)
        return "NULL";

    char* file=(char*)kmalloc(files[i].size+1); //1 byte more than the minimum, to use with \0 if have to be read as txt

    if(file == NULL)
        kpanic("Error while allocating memory for initrd\n", 0x0, __FILE__, __LINE__);
    memcpy(file, &file_datas[header->file_section_offset+files[i].offset], files[i].size);
    return file;
}
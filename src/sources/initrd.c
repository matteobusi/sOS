#include <boot/initrd.h>

#define MAGIC 0x425553

void* mapped_file_ptr;
struct initrd_header fs_header;
struct initrd_file_header fs_files[MAX_INITRD_FILES];

static void fill_header()
{
    memcpy(&fs_header, mapped_file_ptr, sizeof(struct initrd_header));
}

static void fill_files()
{
    //Copies in the files' header all files' haeaders
    //They starts at the end of the inird header, so at sizeof(struct initrd_header)
    memcpy(fs_files, mapped_file_ptr+sizeof(struct initrd_header), sizeof(struct initrd_file_header)*fs_header.n_files);
}

static unsigned int get_offset(const char* filename)
{
    int i;
    for(i=0; i < count_files(); i++)
        if(strcmp(filename, get_file_name_at(i))==0)
            return fs_files[i].offset;
    return -1;
}

int is_initrd(void* memory_mapped_file)
{
    struct initrd_header* header=memory_mapped_file;
    return (header->magic==MAGIC);
}

int init_initrd(void* memory_mapped_file)
{
   //kprintf("MAGIC: 0x%h\n", fs_header->magic);
   mapped_file_ptr=memory_mapped_file;
   if(!is_initrd(mapped_file_ptr))
       return -1;
   fill_header(); 
   //fill files's headers...
   fill_files();
   return 0;
}
//fill the vector with pts to file names and return their number
int list(char vector[][MAX_FILE_NAME_LEN])
{
    int i;
    for(i=0; i < fs_header.n_files; i++)
        strcpy(vector[i], get_file_name_at(i));
    return fs_header.n_files;
}

//file_name -> file to read
//files -> files' headers
//header -> initrd header
//datas -> files contents

//Returns a dynamic allocated ptr to a chunck of memory containing the file
void read(void* dest_buffer, const char* filename)
{
    unsigned int offset=get_offset(filename);
    if(offset==-1)
        return; //ERROR!
    memcpy(dest_buffer, mapped_file_ptr+fs_header.file_section_offset+offset, get_file_size(filename));
}

unsigned int count_files()
{
    return fs_header.n_files;
}

char* get_file_name_at(int index)
{
    if(index >= count_files() || index < 0) return (char*)NULL;
    return fs_files[index].filename;
}
int get_file_size(const char* filename)
{
    int i;
    for(i=0; i < count_files(); i++)
        if(strcmp(filename, get_file_name_at(i))==0)
            return fs_files[i].size;
    return -1;
}

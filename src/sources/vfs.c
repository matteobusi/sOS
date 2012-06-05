#include <fs/vfs.h>

struct FILE* fopen(char* filename)
{
    struct FILE* f=(struct FILE*)kmalloc(sizeof(struct FILE));
    
    if(strcmp(filename, "stdin")==0)
    {
        f->type=STD_IN;
        f->buffer=(char*)kmalloc(STD_IO_SZ);
        f->file_p=0;
        f->sz=0;
        memset(f->buffer, 0, STD_IO_SZ);
    }
    else if(strcmp(filename, "stdout")==0)
    {
        f->type=STD_OUT;
        f->buffer=(char*)kmalloc(STD_IO_SZ);
        f->file_p=0;
        f->sz=0;
        memset(f->buffer, 0, STD_IO_SZ);
    }
    else
    {
        int sz;
        if((sz=get_file_size(filename))==-1)
            return (struct FILE*)NULL; // File not found!
        f->sz=sz;
        f->buffer=(char*)kmalloc(f->sz);
        f->file_p=0;
        read((void*)f->buffer, filename);
        f->type=STD_FILE;
    }
    strcpy(f->filename, filename);
    return f;
}

void fclose(struct FILE* handler)
{
    kfree(handler->buffer,__FILE__, __LINE__);
    kfree(handler,__FILE__, __LINE__);
}

char fgetc(struct FILE* handler)
{    
    if(handler->sz==0)
        return '\0';
    
    char ch=handler->buffer[handler->file_p];
    handler->buffer[handler->file_p]='\0';
    handler->file_p++;
    handler->file_p%=handler->sz;
    return ch;
}

void fputc(char c, struct FILE* handler)
{
    handler->buffer[(++handler->file_p)%(++handler->sz)]=c;
}

void fwrite(char* str, struct FILE* handler)
{
    strcpy(handler->buffer, str);
    handler->sz+=strlen(str);
}

void fread(char* dest, unsigned int count, struct FILE* handler)
{
    int i;
    for(i=0; i < count; i++)
        dest[i]=fgetc(handler);
}
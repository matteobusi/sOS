/* 
 * File:   vfs.h
 * Author: caos
 *
 * Created on May 15, 2012, 12:12 PM
 */

#ifndef VFS_H
#define	VFS_H

#include <memory.h>

#define STD_IN 0
#define STD_OUT 1
#define STD_FILE 2

#define STD_IO_SZ 1000

struct FILE
{
    char filename[100];
    unsigned int type;
    char* buffer;
    unsigned int file_p;
    unsigned int sz;
};

struct FILE* fopen(char* filename);
void fclose(struct FILE* handler);
void fwrite(char* str, struct FILE* handler);
void fputc(char c, struct FILE* handler);
char fgetc(struct FILE* handler);
void fread(char* dest, unsigned int count, struct FILE* handler);

#endif	/* VFS_H */


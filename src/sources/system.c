#include <clib/system.h>

void* memcpy(void* dest, const void* src, int count)
{
    const char *sp = (const char *)src;
    char *dp = (char *)dest;
    for(;count > 0; count--)
        *dp++ = *sp++;
    return dest;
}

void* memset(void* dest, char val, int count)
{
    char* v = (char*) dest;
    for(;count>0; count--)
       *v++ = val;
    return dest;
}

unsigned short* memsetw(unsigned short* dest, unsigned short val, int count)
{
    int i;
    for(i=0; i < count; i++)
        dest[i] = val;
    return dest;
}

int strlen(const char* str)
{
    int i=0;
    while(*(str++) != '\0')
        i++;
    return i;
}

void strcpy(char* dest, const char* source)
{
    while(*source != '\0')
        *dest++ = *source++;
    *dest = '\0';
}

int strcmp(const char* a, const char* b)
{
    while(*a == *b && *a != '\0' && *b != '\0')
    {
        a++;
        b++;
    }
    return (*a)-(*b);
}

void strcat(char* dest, const char* source)
{
    while(*dest++ != '\0');

    while(*source != '\0')
        *dest++ = *source++;

    *dest = '\0';
}

void strrev(char* str)
{
    int i;
    int len = strlen(str);
    char tmp;
    for(i=0; i < len/2; i++)
    {
        tmp = str[i];
        str[i] = str[(len-1)-i];
        str[(len-1)-i] = tmp;
    }
}

unsigned char inportb (unsigned short _port)
{
    unsigned char rv;
    __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
    return rv;
}

void outportb (unsigned short _port, unsigned char _data)
{
    __asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}

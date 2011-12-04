#ifndef SYSTEM_H_INCLUDED
#define SYSTEM_H_INCLUDED

/*
    In this file are defined all the os basic functions:

    #Memory functions
    memcpy
    memset
    memsetw

    #Strings functions
    strcpy
    strlen
    strcmp
    strcat

    #IO functions
    inportb
    outportb
*/

extern void* memcpy(void* dest, const void* src, int count);
extern void* memset(void* dest, char val, int count);
extern unsigned short* memsetw(unsigned short* dest, unsigned short val, int count);

extern int strlen(const char* str);
extern void strcpy(char* dest, const char* source);
extern int strcmp(const char* a, const char* b);
extern void strcat(char* dest, const char* source);
extern void strrev(char* dest);

extern unsigned char inportb (unsigned short _port);
extern void outportb (unsigned short _port, unsigned char _data);

#endif // SYSTEM_H_INCLUDED

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

void* memcpy(void* dest, const void* src, int count);
void* memset(void* dest, char val, int count);
unsigned short* memsetw(unsigned short* dest, unsigned short val, int count);

int strlen(const char* str);
void strcpy(char* dest, const char* source);
int strcmp(const char* a, const char* b);
void strcat(char* dest, const char* source);
void strrev(char* dest);

unsigned char inportb (unsigned short _port);
void outportb (unsigned short _port, unsigned char _data);

int isdigit(char c);

// kmalloc used! memory to be freed
int itoa(int number, int base, char* dest);
int utoa(unsigned int number, int base, char* dest);

int atoi(char* str, int* len);

int ends_with(const char* src, const char* s);

#endif // SYSTEM_H_INCLUDED

//Automatically included in system.h
#ifndef COMMONS_INCLUDED
#define COMMONS_INCLUDED
#define NULL ((char*) 0)

typedef unsigned int uint32;
typedef int int32;
typedef unsigned short uint16;
typedef short int16;
typedef unsigned char ubyte;
typedef char byte;

struct registers{
    unsigned int ds;
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax; //All 32 bits registers pushed by pusha[d]
    unsigned int int_no, err_no;
    unsigned int eip, cs, eflags, useresp, ss; 
};

#endif // COMMONS_INCLUDED

#ifndef PANIC_H
#define	PANIC_H

#include <../clib.h>

//Panic functions
void kpanic(char* error_string, int address_violation, char* file, int line);
void kexception(int err_no, int int_no);
#endif	/* PANIC_H */


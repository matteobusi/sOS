#ifndef STDARG_H_INCLUDED
#define STDARG_H_INCLUDED

typedef char *va_list;

#define __vasz(x)		((sizeof(x)+sizeof(int)-1) & ~(sizeof(int) -1))

#define va_start(ap, parmN)	((ap) = (va_list)&parmN + __vasz(parmN))
#define va_arg(ap, type)      \
  (*((type *)((va_list)((ap) = (void *)((va_list)(ap) + __vasz(type))) \
						    - __vasz(type))))
#define va_end(ap)

#endif /*STDARG_H_INCLUDED*/

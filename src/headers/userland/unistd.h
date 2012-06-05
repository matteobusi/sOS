/* 
 * File:   unistd.h
 * Author: caos
 *
 * Created on May 13, 2012, 2:29 PM
 */

#ifndef UNISTD_H
#define	UNISTD_H

/*
    This file defines all system calls needed by users applications and provided by the kernel, if dynamic calls exists please provide a similar interface to link against user's source!
*/
#define DEFN_SYSCALL(fn, num) \
int sys_##fn() \
{ \
  int a; \
  asm volatile("int $0x80" : "=a" (a) : "0" (num)); \
  return a; \
}

#define DEFN_SYSCALL1(fn, num, P1) \
int sys_##fn(P1 p1) \
{ \
  int a; \
  asm volatile("int $0x80" : "=a" (a) : "0" (num), "b" ((int)p1)); \
  return a; \
}

#define DEFN_SYSCALL2(fn, num, P1, P2) \
int sys_##fn(P1 p1, P2 p2) \
{ \
  int a; \
  asm volatile("int $0x80" : "=a" (a) : "0" (num), "b" ((int)p1), "c"((int)p2)); \
  return a; \
}


#define DEFN_SYSCALL3(fn, num, P1, P2, P3) \
int sys_##fn(P1 p1, P2 p2, P3 p3) \
{ \
  int a; \
  asm volatile("int $0x80" : "=a" (a) : "0" (num), "b" ((int)p1), "c" ((int)p2), "d"((int)p3)); \
  return a; \
}

/* 
    Here there are typedefs for syscalls, many of them are also present in the kernel source, so, if needed change both of them 

    This is the standard interface for SoS system calls:
    here each statically defined system call is declared via a function.
    
    The first, and most important, syscall accessible to any process identified as a 
    server or a driver is add sys_addcall(...) returning an integer, identifing new call ID:
    in case of errors please read the documentation.
    
    ID=0
*/
DEFN_SYSCALL1(addcall, 0, void*);

/*
  These calls are temporary, to be replaced by when a VFS will be present!
  sys_print(stdout, string)
 */
DEFN_SYSCALL1(sprint, 1, const char*);
DEFN_SYSCALL2(dprint, 2, int, int);

DEFN_SYSCALL(get_pid, 5);

/* 
   System calls for message passing:
      + sys_asend: has been made to allow users to pass messages between processes
      + sys_arec: blocks the process until the requested response arrives
*/

DEFN_SYSCALL3(asend, 3, unsigned int,struct message*, unsigned int);
DEFN_SYSCALL1(arec, 4, unsigned int);

#endif	/* UNISTD_H */


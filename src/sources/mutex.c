#include <ipc/mutex.h>

LOCK* init_mutex()
{
    LOCK* l=(LOCK*)kmalloc(sizeof(LOCK));
    l->entries=l->exits=0;
    return l;
}

void end_lock(LOCK* d)
{
    kfree(d,__FILE__, __LINE__);
}

inline int fetch_and_add(int * variable, int value)
{
    asm volatile("lock; xaddl %%eax, %2;":"=a" (value) : "a" (value), "m" (*variable):"memory");
    return value;
}

void lock(LOCK* l)
{
     int ticket = fetch_and_add(&(l->entries), 1);
     while (ticket != l->exits)
         continue; // or anything else you can do while waiting for your turn
}

void unlock(LOCK* l)
{
     l->exits++;
}
 
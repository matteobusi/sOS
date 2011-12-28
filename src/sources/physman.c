#include <memory/physman.h>


extern unsigned int kend;
extern unsigned int kernel_heap;
unsigned int init_address = (unsigned int)&kend;
extern struct page_dir* kernel_dir;

unsigned int kmalloc_aligned_phys(unsigned int size, unsigned char aligned, unsigned int* phys)
{
    //if the heap is initialized it uses it...
    if(kernel_heap != 0)
    {
        void* addr = alloc(size, aligned);
        if(phys != 0)
        {
            struct page* pg = get_page((unsigned int)addr, 0, kernel_dir);
            *phys = pg->frame*PAGE_SIZE + ((unsigned int)addr&0xFFF);
        }
        return (unsigned int)addr;
    }

    if(aligned && (init_address & 0xFFFFF000))
    {
        init_address &= 0xFFFFF000;
        init_address += PAGE_SIZE;
    }
    if(phys)
        *phys = init_address;

    unsigned int tmp = init_address;
    init_address += size;
    return tmp;
}

unsigned int kmalloc(unsigned int size)
{
    return kmalloc_aligned_phys(size, 0,0);
}

unsigned int kmalloc_a(unsigned int size)
{
    return kmalloc_aligned_phys(size, 1, 0);
}

unsigned int kmalloc_p(unsigned int size, unsigned int* phys)
{
    return kmalloc_aligned_phys(size, 0, phys);
}

unsigned int kmalloc_ap(unsigned int size, unsigned int* phys)
{
    return kmalloc_aligned_phys(size, 1, phys);
}

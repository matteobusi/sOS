#ifndef PAGING_H_INCLUDED
#define PAGING_H_INCLUDED

#include <../interrupts.h>
#include <../utilities.h>

#include <physman.h>
#include <heap.h>

#define BIT2INDEX(a) (a/(4*8))
#define BIT2OFFSET(a) (a%(8*4))
#define ADDRESS(i,j) (i*4*8+j)


#define PAGE_SIZE 4096
//paging
struct page
{
    unsigned int present :1;
    unsigned int readwrite: 1;
    unsigned int user: 1;
    unsigned int accessed: 1;
    unsigned int dirty: 1;
    unsigned int unused: 7;
    unsigned int frame: 20;
};

struct page_table
{
    struct page pages[1024];
};

struct page_dir
{
    struct page_table* tables[1024];

    unsigned int physical_table[1024];

    unsigned int phys;
};

//inits paging module
void init_paging(unsigned int size);

//loads in memory the passed directory
void load_dir(struct page_dir* directory);

//gets a page
//at <address>
//new if <newpage> == 1
//in <directory> dir
struct page* get_page(unsigned int address, int new_page, struct page_dir* directory);

struct page_dir *clone_directory(const struct page_dir* src);

#endif // PAGING_H_INCLUDED

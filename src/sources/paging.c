
#include <memory/paging.h>

extern void copy_page_physical();

struct page_dir* kernel_dir=0;
struct page_dir* current_dir=0;

extern unsigned int init_address; //in physman.c

static char* page_fault_msg[]=
{
        "Supervisory process tried to read a non-present page entry",\
        "Supervisory process tried to read a page and caused a protection fault",\
        "Supervisory process tried to write to a non-present page entry",\
        "Supervisory process tried to write a page and caused a protection fault",\
        "User process tried to read a non-present page entry",\
        "User process tried to read a page and caused a protection fault",\
        "User process tried to write to a non-present page entry",\
        "User process tried to write a page and caused a protection fault"\
};

unsigned int* frames;
unsigned int n_frames;

static void set_frame(unsigned int address)
{
    unsigned int frame = address / PAGE_SIZE;
    frames[BIT2INDEX(frame)] |= (0x1 << BIT2OFFSET(frame));
}

static void clear_frame(unsigned int address)
{
    unsigned int frame = address / PAGE_SIZE;
    frames[BIT2INDEX(frame)] &= ~(0x1 << BIT2OFFSET(frame));
}

static int get_bit_state(unsigned int index, unsigned int bit)
{
    return (frames[index]&(0x1 << bit));
}

static unsigned int first_free_frame()
{
    int i, j;
    for(i=0; i < BIT2INDEX(n_frames); i++)
        //checks if the current set of 32 frames has at least 1 free frame
        if(frames[i] != 0xFFFFFFFF)
            //check what's the free frame and return it's address
            for(j=0; j< 32; j++)
                if(!get_bit_state(i, j))
                    return ADDRESS(i, j);
    return -1;
}

static void page_fault(struct registers* regs)
{
    unsigned int exc_addr;
    //Gets the exception unsigned int from cr2 register
    asm volatile("mov %%cr2, %0": "=r"(exc_addr));
    //analyzes the cases:
    int usr,rw, p;
    p = !(regs->err_no & 0x1);
    rw = regs->err_no & 0x2;
    usr = regs->err_no & 0x4;
    int n = (usr << 3)|(rw<<2)|p;
    //prints the error message
    kpanic(page_fault_msg[n%8], tasks->current->value->pid, __FILE__, __LINE__);
    for(;;);
}

void alloc_frame(struct page* p, int kernel, int writable)
{
    if(p->frame != 0) return;

    unsigned int first_free = first_free_frame();
    if(first_free == (unsigned int)-1)
        kpanic("Error while allocating memory!\n", 0x0, __FILE__, __LINE__);

    set_frame(first_free*PAGE_SIZE);

    p->present = 1;
    p->readwrite = (writable)?1:0;
    p->user = (kernel)?0:1;
    p->frame = first_free;
}

void free_frame(struct page* p)
{
    unsigned int frame;
    if(!(frame=p->frame)) return;

    clear_frame(frame);
    frame = 0;
}
static struct page_table* clone_table(struct page_table* src, unsigned int* phys)
{
    //creates the new table...
    struct page_table* new_table = (struct page_table*)kmalloc_ap(sizeof(struct page_table), phys);

    if(new_table==(struct page_table*)NULL)
        kpanic("Error allocating space for a new table!", 0x0, __FILE__, __LINE__);
    
    //zeroes it...
    memset(new_table, 0, sizeof(struct page_dir));
    
    //and now, the same as clone_directory, we copy all non-empty frames...
    int i;
    for(i=0; i < 1024;i++)
    {
        if(src->pages[i].frame)
        {        
            //now alloc a new frame...
            alloc_frame(&(new_table->pages[i]), 0,0); //user, read-only
            
            //copy flags from source to destination.
            new_table->pages[i].present = src->pages[i].present;   
            new_table->pages[i].readwrite = src->pages[i].readwrite;
            new_table->pages[i].user = src->pages[i].user;
            new_table->pages[i].accessed = src->pages[i].accessed;
            new_table->pages[i].dirty = src->pages[i].dirty;

            //now physically copy the data from src to new_table frame...
            copy_page_physical(src->pages[i].frame*PAGE_SIZE, new_table->pages[i].frame*PAGE_SIZE); 
        }
    }
    return new_table;
}

struct page_dir *clone_directory(const struct page_dir* src)
{
    unsigned int phys;
    struct page_dir* new_dir=(struct page_dir*)kmalloc_ap(sizeof(struct page_dir), &phys); //Gets a new address, aligned in memory and its physical address
    
    if(new_dir==(struct page_dir*)NULL)
        kpanic("Error while creating a new paging directory :'(", 0x0, __FILE__, __LINE__);

    memset(new_dir, 0, sizeof(struct page_dir)); //zeroes the memory...
    //get the offset of the physical addresses of pages tables from the start of the dir...
    unsigned int offset = (unsigned int)new_dir->physical_table - (unsigned int)new_dir;

    //now, the real physical address of page tables is phys (the real position of the new dir + the offset)
    new_dir->phys = offset+phys;
    
    //now we have to copy each page table from src to the new dir, iff that's not the kernel dir...
    int i;
    for(i=0; i < 1024; i++)
    {
        if(!src->tables[i]) //if the current table it's not 0, we should either copy or link the table
            continue;
        
        /*
         * Now, if the current table is also in the kernel directory we should link it (we don't have to copy the kernel code and data, but only to link them)
         * else we copy the table...
         */

        if(kernel_dir->tables[i]==src->tables[i])
        {
            //our table is also in the kernel dir, link!
            new_dir->tables[i]=src->tables[i];
            new_dir->physical_table[i] = src->physical_table[i];
        }            
        else
        {
            unsigned int phys;
            new_dir->tables[i]=clone_table(src->tables[i], &phys);
            new_dir->physical_table[i] = phys | 0x07; //0x07 -> present, rw, usr mode
        }
    }
    return new_dir;    
}

void init_paging(unsigned int size)
{
    /* Gets number of frames */
    n_frames = size/PAGE_SIZE;
    
    /* Initialize the frame allocator */
    frames = (unsigned int*)kmalloc(BIT2INDEX(n_frames));
    memset(frames, 0, BIT2INDEX(n_frames));

    /* Creates the page directory */
    kernel_dir = (struct page_dir*) kmalloc_a(sizeof(struct page_dir));
    
    /* Sets everything to 0 */
    memset(kernel_dir, 0, sizeof(struct page_dir));    
    kernel_dir->phys = (unsigned int)kernel_dir->physical_table;
    
    /* Then sets current directory to the kernel one */
    current_dir = kernel_dir;
    int i;

    /* From HEAP_BASE to HEAP_END maps */
    for(i=HEAP_BASE; i<HEAP_END; i+=PAGE_SIZE)
        get_page(i, 1, kernel_dir);

    /* Identity map from 0x0 to initial address */
    i=0;
    while(i<init_address+PAGE_SIZE)
    {
        alloc_frame(get_page(i, 1, kernel_dir), 0, 0);
        i+=PAGE_SIZE;
    }
    
    for(i=HEAP_BASE; i<HEAP_END; i+=PAGE_SIZE)
        alloc_frame(get_page(i, 1, kernel_dir), 0,0);
 
    /* Adds fault handler for page fault */   
    add_handler(14, page_fault);
    
    /* Loads and enable the kernel directory */
    load_dir(kernel_dir);
    init_heap();
    load_dir(current_dir);
}

void load_dir(struct page_dir* directory)
{
   current_dir=directory;
   unsigned int address=directory->phys;
   asm volatile("mov %0, %%cr3":: "r"(address));
   unsigned int cr0;
   asm volatile("mov %%cr0, %0": "=r"(cr0));
   cr0 |= 0x80000000; // Enable paging!
   asm volatile("mov %0, %%cr0":: "r"(cr0));
}

struct page* get_page(unsigned int address, int new_page, struct page_dir* directory)
{
    unsigned int index = address/PAGE_SIZE;
    unsigned int tbl_index = index/1024;
    if(directory->tables[tbl_index])
        return &directory->tables[tbl_index]->pages[index%1024];
    else if(new_page)
    {
        unsigned int tmp;
        directory->tables[tbl_index] = (struct page_table*) kmalloc_ap(sizeof(struct page_table), &tmp);

        memset(directory->tables[tbl_index], 0, PAGE_SIZE);

        directory->physical_table[tbl_index] = tmp | 0x7;

        return &directory->tables[tbl_index]->pages[index%1024];
    }
    else
        return 0;
}
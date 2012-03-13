#include <memory/heap.h>

//points *always* at the last allocated block
struct mem_block_header* kernel_heap=(struct mem_block_header*)NULL;
//
int free_blocks=0;

//given the position of the header it calculates the necessary offset to align the chunk
static unsigned int get_offset(struct mem_block_header* address)
{
    if((((unsigned int)(address+1)) % PAGE_SIZE) != 0)
        return PAGE_SIZE - (unsigned int)(address+1)%PAGE_SIZE;

    return 0;
}

void init_heap() {
    kernel_heap = (struct mem_block_header*)HEAP_BASE;
    kernel_heap->previous = (struct mem_block_header*)NULL;
    kernel_heap->next=(struct mem_block_header*)NULL;
    kernel_heap->is_free=1;
    kernel_heap->size=-1;
    kernel_heap->magic=HEAP_MAGIC;
    free_blocks++;
}

static void* add_new(unsigned int size, char aligned) {
    kernel_heap->next = (struct mem_block_header*)((unsigned int)kernel_heap+kernel_heap->size+sizeof(struct mem_block_header));
    struct mem_block_header* prev = kernel_heap;

    kernel_heap = kernel_heap->next;
    unsigned int offset = 0;

    if(aligned)
        offset = get_offset(kernel_heap);

    kernel_heap->size=offset+size;
    kernel_heap->next = (struct mem_block_header*) NULL;
    kernel_heap->previous=prev;
    kernel_heap->is_free =0;
    kernel_heap->magic = HEAP_MAGIC;

    return ((void*)(kernel_heap+1))+offset;
}



static void* best_fit(unsigned int size, char aligned) {
    //starts from the first block
    struct mem_block_header* current_block = (struct mem_block_header*)HEAP_BASE;
    struct mem_block_header* best=current_block;
    unsigned int best_offset=0;

    //if no blocks in the heap, add the first with the needed offset and go on
    if(kernel_heap->size == -1)
        kernel_heap->size = size + get_offset(kernel_heap);

    while(current_block != (struct mem_block_header*)NULL)
    {
        int offset=0;
        //if the memory have to be memory aligned we need an additional chunck of memory (maybe 0 :)
        if(aligned)
            offset = get_offset(current_block);


        //a small optimization
        if(current_block->is_free && current_block->size== size + offset)
        {
            best = current_block;
            best_offset=offset;
            break;
        }
        else if(current_block->is_free && current_block->size >= size + offset && current_block->size <= best->size)
        {
            best = current_block;
            best_offset=offset;
        }
        current_block = current_block->next;
    }

    if(best->size < size + best_offset)
        return add_new(size, aligned);

    best->is_free=0;
    free_blocks--;
    return ((void*)(best+1))+best_offset;
}

void* alloc(unsigned int size, char aligned)
{
    if(size+sizeof(struct mem_block_header)+PAGE_SIZE > HEAP_END - ((unsigned int)kernel_heap+kernel_heap->size+sizeof(struct mem_block_header)))
        return NULL;
    if(free_blocks == 0)
        return add_new(size, aligned);
    else
        return best_fit(size, aligned);
}

void kfree(void* ptr) {
    if(ptr == NULL) return;

    struct mem_block_header* head = (struct mem_block_header*)(ptr-sizeof(struct mem_block_header));

    //TODO: find a better way...althought this is O(PAGE_SIZE), at most uses PAGE_SIZE steps
    if((int)ptr%PAGE_SIZE == 0)
    {
        //searches the header...
        int i;
        for(i=0; i < PAGE_SIZE; i++)
            if(((struct mem_block_header*)(ptr-i-sizeof(struct mem_block_header)))->magic==HEAP_MAGIC)
            {
                head=(struct mem_block_header*)(ptr-i-sizeof(struct mem_block_header));
                break;
            }
    }

    if(head->magic != HEAP_MAGIC) 
        kpanic("Error while freeing memory: currupted or damaged memory!", (unsigned int)head, __FILE__, __LINE__);

    //now frees the chunk
    head->is_free=1;
    free_blocks++;
}
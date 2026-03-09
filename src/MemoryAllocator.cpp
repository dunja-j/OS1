#include "../h/MemoryAllocator.hpp"

FreeMemoryHeader* MemoryAllocator::head = nullptr;
bool MemoryAllocator::initialized = false;

void MemoryAllocator::initialize()
{
    head = (FreeMemoryHeader*) HEAP_START_ADDR;
    head->size = (size_t)HEAP_END_ADDR - (size_t)HEAP_START_ADDR - 1 - sizeof(FreeMemoryHeader);
    head->next = nullptr;
    initialized = true;
}

void *MemoryAllocator::mem_alloc(size_t size)
{
    if(!initialized) initialize();
    if(size <= 0 || !head) return nullptr;

    FreeMemoryHeader* prev = nullptr;
    FreeMemoryHeader* curr = head;
    size_t sizeInBytes = size * MEM_BLOCK_SIZE;

    while(curr) { // first fit
        if(curr->size >= sizeInBytes) break; //found
        prev = curr;
        curr = curr->next;
    }
    
    if(!curr) return nullptr; //big enough block not found

    if(curr->size - sizeInBytes >= MEM_BLOCK_SIZE + sizeof(FreeMemoryHeader)) { //there is a block remining
        FreeMemoryHeader* reminder = (FreeMemoryHeader*)((size_t)curr + sizeof(FreeMemoryHeader) + sizeInBytes);
        reminder->next = curr->next;
        reminder->size = curr->size - sizeInBytes - sizeof(FreeMemoryHeader);
        curr->size = sizeInBytes;
        if(prev) prev->next = reminder;
        else head = reminder;
    }
    else { //space remining too small or non-existant
        if(prev) prev->next = curr->next;
        else head = head->next;
    }

    return (void*)((size_t)curr + sizeof(FreeMemoryHeader));
}

int MemoryAllocator::mem_free(void *ptr)
{
    FreeMemoryHeader* free = (FreeMemoryHeader*)((size_t)ptr - sizeof(FreeMemoryHeader));
    FreeMemoryHeader* prev = nullptr;
    FreeMemoryHeader* curr = head;

    if ((void*)free < HEAP_START_ADDR || (void*)free >= HEAP_END_ADDR) return -1;

    while(curr) {
        if(curr == free) return -1;
        if(curr > free) break;

        prev = curr;
        curr = curr->next;
    }

    //free between prev and curr;
    if(prev) prev->next = free;
    else head = free;
    free->next = curr;

    if(curr) MemoryAllocator::tryToJoin(free, curr);
    if(prev) MemoryAllocator::tryToJoin(prev, free);

    return 0;
}

size_t MemoryAllocator::mem_get_free_space()
{
    if(!initialized) initialize();

    FreeMemoryHeader* curr = head;
    size_t freeSpace = 0;

    while(curr) {
        freeSpace += curr->size;
        curr = curr->next;
    }

    return freeSpace;
}

size_t MemoryAllocator::mem_get_largest_free_block()
{
    if(!initialized) initialize();

    FreeMemoryHeader* curr = head;
    size_t largest = 0;

    while(curr) {
        if (curr->size > largest) largest = curr->size;
        curr = curr->next;
    }

    return largest;
}

void MemoryAllocator::tryToJoin(FreeMemoryHeader *prev, FreeMemoryHeader *curr)
{
    if ((size_t) prev + sizeof(FreeMemoryHeader) + prev->size == (size_t) curr) {
        prev->size += curr->size + sizeof(FreeMemoryHeader);
        prev->next = curr->next;
    }
}

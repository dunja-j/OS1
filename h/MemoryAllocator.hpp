#ifndef OSPROJEKAT_MEMORYALLOCATOR_HPP
#define OSPROJEKAT_MEMORYALLOCATOR_HPP

#include "../lib/hw.h"

struct FreeMemoryHeader {
    size_t size;          //size of the free block in BYTES not including the header(since that part iz occupied by the header, so it's not free)
    FreeMemoryHeader* next;
};

class MemoryAllocator {
public:

    static void* mem_alloc(size_t size); //in blocks
    static int mem_free(void* ptr);
    static size_t mem_get_free_space();
    static size_t mem_get_largest_free_block();

private:
    static FreeMemoryHeader* head;
    static bool initialized;

    static void initialize();
    static void tryToJoin(FreeMemoryHeader* prev, FreeMemoryHeader* curr);

    MemoryAllocator(const MemoryAllocator&) = delete;
    MemoryAllocator& operator=(const MemoryAllocator&) = delete;

};

#endif //OSPROJEKAT_MEMORYALLOCATOR_HPP

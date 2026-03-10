#include "../lib/console.h"
#include "../h/MemoryAllocator.hpp"

void printString(const char* str)
{
    if (!str) return;

    while (*str != '\0') {
        __putc(*str);
        str++;
    }
}

void printInt(size_t num)
{
    char buffer[20];
    int i = 0;

    if(num == 0){
        __putc('0');
        return;
    }

    while(num > 0){
        buffer[i++] = '0' + (num % 10);
        num /= 10;
    }

    while(i--){
        __putc(buffer[i]);
    }
}

void printNewLine()
{
    __putc('\n');
}

int main()
{
    printString("===== Memory Allocator Test =====\n");

    printString("Initial free space: ");
    printInt(MemoryAllocator::mem_get_free_space());
    printNewLine();

    printString("Largest free block: ");
    printInt(MemoryAllocator::mem_get_largest_free_block());
    printNewLine();
    printNewLine();

    printString("Allocating block A (10 blocks)\n");
    void* A = MemoryAllocator::mem_alloc(10);

    printString("Allocating block B (20 blocks)\n");
    void* B = MemoryAllocator::mem_alloc(20);

    printString("Allocating block C (5 blocks)\n");
    void* C = MemoryAllocator::mem_alloc(5);

    printNewLine();

    printString("Free space after allocations: ");
    printInt(MemoryAllocator::mem_get_free_space());
    printNewLine();

    printString("Largest free block: ");
    printInt(MemoryAllocator::mem_get_largest_free_block());
    printNewLine();
    printNewLine();

    printString("Freeing block B\n");
    int res = MemoryAllocator::mem_free(B);

    printString("mem_free(B) returned: ");
    printInt(res);
    printNewLine();

    printString("Free space after freeing B: ");
    printInt(MemoryAllocator::mem_get_free_space());
    printNewLine();

    printString("Largest free block: ");
    printInt(MemoryAllocator::mem_get_largest_free_block());
    printNewLine();
    printNewLine();

    printString("Allocating block D (15 blocks)\n");
    void* D = MemoryAllocator::mem_alloc(15);

    printString("Free space after allocating D: ");
    printInt(MemoryAllocator::mem_get_free_space());
    printNewLine();
    printNewLine();

    printString("Freeing block A\n");
    MemoryAllocator::mem_free(A);

    printString("Freeing block C\n");
    MemoryAllocator::mem_free(C);

    printString("Freeing block D\n");
    MemoryAllocator::mem_free(D);

    printNewLine();

    printString("Free space after freeing everything: ");
    printInt(MemoryAllocator::mem_get_free_space());
    printNewLine();

    printString("Largest free block: ");
    printInt(MemoryAllocator::mem_get_largest_free_block());
    printNewLine();
    printNewLine();

    printString("Testing invalid free\n");
    int invalid = MemoryAllocator::mem_free((void*)0x12345);

    printString("mem_free(invalid_ptr) returned: ");
    printInt(invalid);
    printNewLine();

    printString("===== Test Finished =====\n");

    return 0;
}
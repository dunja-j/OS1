#include "../lib/console.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/tcb.hpp"
//#include "_new.cpp"
#include "../h/syscall_cpp.hpp"
#include "../h/riscv.hpp"
#include "../test/printing.hpp"


volatile bool mainFinished = false;

void userMainThreadID() {
    printInt(getThreadId());
    int i = 0;
    while(i<10) {
        printString("nit broj ");
        printInt(getThreadId());
        printString(", iteracija: ");
        printInt(i);
        printString("\n\n");
        i++;
    }
}//test is showing that getthreadid successfully dispatches

void userMainWrapper(void*) {
    userMainThreadID();
    mainFinished = true;
}


int main() {
    Riscv::w_stvec((uint64) Riscv::supervisorTrap);

    TCB* userMainThread = nullptr;
    TCB* myMainThread = nullptr;
    //thread_create(&myMainThread, nullptr, nullptr);
    myMainThread = TCB::createThread(nullptr, nullptr, nullptr);

    TCB::running = myMainThread;

    //thread_create(&userMainThread, userMainWrapper, nullptr);
    userMainThread = TCB::createThread(userMainWrapper, nullptr, MemoryAllocator::mem_alloc(DEFAULT_STACK_SIZE));
    while(!mainFinished) {
        printString("main dobio kontrolu \n");
        thread_dispatch();
    }

    delete userMainThread;
    delete myMainThread;

    return 0;
}

/*int main()
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
}*/
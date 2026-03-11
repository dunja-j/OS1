#include "../h/syscall_c.hpp"
#include "../h/MemoryAllocator.hpp"

void *mem_alloc(size_t size)
{
    size_t blocks = (size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE; //rounding up on the num of blocks needed
    __asm__ volatile("mv a1, %0" :: "r" (blocks)); //argument
    __asm__ volatile("mv a0, %0" :: "r" (MEM_ALLOC)); //syscall code 
    __asm__ volatile("ecall");

    uint64 addr; //because all registers are uint64
    __asm__ volatile("mv %0, a0" : "=r" (addr)); //ret value in a0
    return (void*)addr;
}

int mem_free(void *ptr)
{
    __asm__ volatile("mv a1, %0" :: "r" (ptr));
    __asm__ volatile("mv a0, %0" :: "r" (MEM_FREE));
    __asm__ volatile("ecall");

    uint64 ret; //because all registers are uint64
    __asm__ volatile("mv %0, a0" : "=r" (ret)); //ret value in a0
    return (int)ret; //0 if successful -1 if not
}

size_t mem_get_free_space()
{
    __asm__ volatile("mv a0, %0" :: "r" (MEM_GET_FREE_SPACE));
    __asm__ volatile("ecall");

    uint64 freeSpace; //because all registers are uint64
    __asm__ volatile("mv %0, a0" : "=r" (freeSpace)); //ret value in a0
    return (int)freeSpace;
}

size_t mem_get_largest_free_block()
{
    __asm__ volatile("mv a0, %0" :: "r" (MEM_GET_LARGEST_FREE_BLOCK));
    __asm__ volatile("ecall");

    uint64 largest; //because all registers are uint64
    __asm__ volatile("mv %0, a0" : "=r" (largest)); //ret value in a0
    return (int)largest;
}

int thread_create(thread_t *handle, thread_body_t start_routine, void *arg)
{
    void* stack_space = mem_alloc(DEFAULT_STACK_SIZE);//ne znam dal direktno da zovem mem alokator proveri
    __asm__ volatile("mv a4, %0" : : "r" (stack_space));
    __asm__ volatile("mv a3, %0" : : "r" (arg));
    __asm__ volatile("mv a2, %0" : : "r" (start_routine));
    __asm__ volatile("mv a1, %0" : : "r" (handle));
    __asm__ volatile("mv a0, %0" : : "r" (THREAD_CREATE));
    __asm__ volatile("ecall");

    uint64 ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return (int)ret; //if the syscall was successful 0, 1 if unsuccessful
}

int thread_exit()
{
    __asm__ volatile("mv a0, %0" : : "r" (THREAD_EXIT));
    __asm__ volatile("ecall");

    uint64 ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return (int) ret;
}

void thread_dispatch()
{
    __asm__ volatile("mv a0, %0" : : "r" (THREAD_DISPATCH));
    __asm__ volatile("ecall");
}

int sem_open(sem_t *handle, unsigned init)
{
    return 0;
}

int sem_close(sem_t handle)
{
    return 0;
}

int sem_wait(sem_t id)
{
    return 0;
}

int sem_signal(sem_t id)
{
    return 0;
}

int time_sleep(time_t period)
{
    return 0;
}

char getc()
{
    return 0;
}

void putc(char c)
{
}

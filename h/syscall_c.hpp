#ifndef OS1_SYSCALL_C_HPP
#define OS1_SYSCALL_C_HPP

#include "../lib/hw.h"

// Forward declaration to avoid circular dependency with tcb.hpp
class TCB;

enum codes {
    MEM_ALLOC = 0x01, MEM_FREE, MEM_GET_FREE_SPACE, MEM_GET_LARGEST_FREE_BLOCK,
    THREAD_CREATE = 0x11, THREAD_EXIT, THREAD_DISPATCH, THREAD_ID,
    SEM_OPEN = 0x21, SEM_CLOSE, SEM_WAIT, SEM_SIGNAL,
    TIME_SLEEP = 0x31,
    GETC = 0x41, PUTC
};

void* mem_alloc (size_t size);
int mem_free (void*);
size_t mem_get_free_space();
size_t mem_get_largest_free_block();

//class _thread;
typedef TCB* thread_t;
typedef void (*thread_body_t) (void*);

int thread_create (thread_t* handle, thread_body_t start_routine, void* arg);
int thread_exit ();
void thread_dispatch ();
//-------------------------MODIFICATION--------------------------------------
int getThreadId();
//----------------------------------------------------------------------------

class _sem;
typedef _sem* sem_t;

int sem_open (sem_t* handle, unsigned init);
int sem_close (sem_t handle);
int sem_wait (sem_t id);
int sem_signal (sem_t id);

typedef unsigned long time_t;
int time_sleep (time_t);

const int EOF = -1;
char getc ();
void putc (char);

#endif //OSPROJEKAT_SYSCALL_C_HPP
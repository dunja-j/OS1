#include "../h/Sem.hpp"

int Sem::sem_signal()
{
    if(val++ < 0) {
        thread_t thread = blocked.removeFirst();
        thread->blocked = false;
        Scheduler::put(thread);
    }
    return 0;
}

int Sem::sem_wait()
{
    if(--val < 0) {
        TCB::running->blocked = true;
        blocked.addLast(TCB::running);
        TCB::dispatch();
    }
    return 0;
}

int Sem::sem_close()
{
    while(blocked.peekFirst()) {
        thread_t thread = blocked.removeFirst();
        thread->blocked = false;
        Scheduler::put(thread);
    }
    return 0;
}

int Sem::sem_open(sem_t* handle, unsigned init)
{
    if(!handle) return -1;
    *handle = new Sem(init);
    return 0;
}

Sem::Sem(int val) : val(val) {}

Sem::~Sem()
{
    sem_close();
}

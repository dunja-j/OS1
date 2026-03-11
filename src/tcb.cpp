#include "../h/tcb.hpp"
#include "../h/riscv.hpp"

thread_t TCB::running = nullptr;
time_t TCB::time = 0;

thread_t TCB::createThread(thread_body_t start_routine, void* arg, void* stack_space)
{
    return new TCB(start_routine, arg, stack_space);
}

void TCB::dispatch()
{
    TCB *old = running;
    if (!old->finished && !old->blocked) { Scheduler::put(old); }
    running = Scheduler::get();
    TCB::contextSwitch(&old->context, &running->context);
}

void TCB::threadWrapper()
{
    Riscv::popSppSpie();
    running->start_routine(running->arg);
    thread_exit();
}

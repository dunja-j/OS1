#include "../h/tcb.hpp"
#include "../h/riscv.hpp"

thread_t TCB::running = nullptr;
time_t TCB::time = 0;

TCB *TCB::createThread()
{
    //return new TCB(body, TIME_SLICE);
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

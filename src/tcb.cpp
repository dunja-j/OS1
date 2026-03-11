#include "../h/tcb.hpp"
#include "../h/riscv.hpp"

thread_t TCB::running = nullptr;
time_t TCB::time = 0;

TCB::TCB(thread_body_t start_routine, void *arg, void *stack_space)
: start_routine(start_routine), arg(arg), stack(stack_space) {
    context.sp = (uint64) stack_space + DEFAULT_STACK_SIZE;
    context.ra = (uint64) threadWrapper;
    Scheduler::put(this); //do ovde sam stigla
}

TCB *TCB::createThread()
{
    //return new TCB(body, TIME_SLICE);
}

void TCB::dispatch()
{
    TCB *old = running;
    if (!old->isFinished()) { Scheduler::put(old); }
    running = Scheduler::get();

    TCB::contextSwitch(&old->context, &running->context);
}

void TCB::threadWrapper()
{
    Riscv::popSppSpie();
    running->body();
    running->setFinished(true);
    TCB::yield();
}

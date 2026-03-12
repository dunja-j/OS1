#ifndef OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP
#define OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP

#include "../lib/hw.h"
#include "scheduler.hpp"
#include "../h/syscall_c.hpp"
//#include "../src/_new.cpp"

// Thread Control Block
class TCB
{
public:
    static thread_t running;
    static time_t time;

    //TCB(thread_body_t start_routine, void* arg, void* stack_space);
    ~TCB() { uint64* uint64_stack = (uint64*)stack ; delete uint64_stack; }

    bool finished = false;
    //bool waiting = false;
    //bool interrupted = false;
    bool blocked = false;

    static thread_t createThread(thread_body_t start_routine, void* arg, void* stack_space);

    //static int exit();
    static void dispatch();

private:
    /*TCB(Body body, uint64 timeSlice) :
            body(body),
            stack(body != nullptr ? new uint64[STACK_SIZE] : nullptr),
            context({(uint64) &threadWrapper,
                     stack != nullptr ? (uint64) &stack[STACK_SIZE] : 0
                    }),
            timeSlice(timeSlice),
            finished(false)
    {
        if (body != nullptr) { Scheduler::put(this); }
    }*/
    
    thread_body_t start_routine = nullptr;
    void* arg = nullptr;
    void* stack = nullptr;
    struct Context
    {
        uint64 ra;
        uint64 sp;
    };
    Context context;

    friend class Riscv;

    static void threadWrapper();

    static void contextSwitch(Context *oldContext, Context *runningContext);

    TCB(thread_body_t start_routine, void* arg, void* stack_space):
            start_routine(start_routine),
            arg(arg),
            stack(start_routine != nullptr? stack_space: nullptr),
            context(
                    {start_routine != nullptr? (uint64)&threadWrapper : 0,
                    stack != nullptr? (uint64)stack + DEFAULT_STACK_SIZE : 0}
            )
    {
        if(start_routine != nullptr) Scheduler::put(this);
    }


};

#endif //OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP

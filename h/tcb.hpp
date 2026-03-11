#ifndef OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP
#define OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP

#include "../lib/hw.h"
#include "scheduler.hpp"
#include "../h/syscall_c.hpp"

// Thread Control Block
class TCB
{
public:
    static thread_t running;
    static time_t time;

    TCB(thread_body_t start_routine, void* arg, void* stack_space);
    ~TCB() { delete[] stack; }

    bool finished = false;
    bool waiting = false;
    bool interrupted = false;

    static thread_t createThread();

    static int exit();
    static void dispatch();

private:
    TCB(Body body, uint64 timeSlice) :
            body(body),
            stack(body != nullptr ? new uint64[STACK_SIZE] : nullptr),
            context({(uint64) &threadWrapper,
                     stack != nullptr ? (uint64) &stack[STACK_SIZE] : 0
                    }),
            timeSlice(timeSlice),
            finished(false)
    {
        if (body != nullptr) { Scheduler::put(this); }
    }

    struct Context
    {
        uint64 ra;
        uint64 sp;
    };

    thread_body_t start_routine = nullptr;
    void* arg = nullptr;
    void* stack = nullptr;
    Context context;

    friend class Riscv;

    static void threadWrapper();

    static void contextSwitch(Context *oldContext, Context *runningContext);

};

#endif //OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP

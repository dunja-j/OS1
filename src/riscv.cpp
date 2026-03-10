//
// Created by marko on 20.4.22..
//

#include "../h/riscv.hpp"
#include "../h/tcb.hpp"
#include "../lib/console.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/syscall_c.hpp"
#include "../h/print.hpp"

void Riscv::popSppSpie()
{
    __asm__ volatile("csrc sstatus, %0"::"r"(1<<8)); //switching to user mode since this the function used before calling the body function of a thread
    __asm__ volatile("csrw sepc, ra"); //setting up the return addr of the syscall to be the ret val of the function
    __asm__ volatile("sret"); //the only way to exit system privilaged mode
}

void Riscv::handleSupervisorTrap() {
    volatile uint64 scause = Riscv::r_scause();

    volatile uint64 sepc = Riscv::r_sepc();
    volatile uint64 sstatus = Riscv::r_sstatus();

    if (scause == USER_ECALL || scause == SYSTEM_ECALL) {
        uint64 volatile code;
        __asm__ volatile("ld %0, 10*8(fp)" : "=r"(code)); //reading a0 from the stack using frame pointer since the code of the syscall is in a0
        sepc += 4; //ecall syscall leaves the return adress to ITSELF and not the next instruction, and its 4 bytes long so this sets the return addr of the syscall to the line after ecall

        switch (code) {
            case MEM_ALLOC: {
                uint64 blocks;
                __asm__ volatile("ld %0, 11*8(fp)" : "=r"(blocks)) //reading a1 for the argument
                void* addr = MemoryAllocator::mem_alloc(blocks);
                __asm__ volatile("sd %0, 10*8(fp)" :: "r"(addr)); //storing the return value of the syscall in a0
                break;
            }
            case MEM_FREE: {
                uint64 addr;
                __asm__ volatile("ld %0, 11*8(fp)" : "=r"(addr)) //reading a1 for the argument
                int ret = MemoryAllocator::mem_free((void*)addr);
                __asm__ volatile("sd %0, 10*8(fp)" :: "r"(ret)); //storing the return value of the syscall in a0
                break;
            }
            case MEM_GET_FREE_SPACE: {
                size_t freeSpace = MemoryAllocator::mem_get_free_space();
                __asm__ volatile("sd %0, 10*8(fp)" :: "r"(freeSpace)); //storing the return value of the syscall in a0
                break;
            }
            case MEM_GET_LARGEST_FREE_BLOCK: {
                size_t largest = MemoryAllocator::mem_get_largest_free_block();
                __asm__ volatile("sd %0, 10*8(fp)" :: "r"(largest)); //storing the return value of the syscall in a0
                break;
            }
            /*case THREAD_CREATE: {
                uint64 handle = Riscv::r_user_reg(A1);
                uint64 start_routine = Riscv::r_user_reg(A2);
                uint64 arg = Riscv::r_user_reg(A3);
                uint64 stack_space = Riscv::r_user_reg(A4);

                if ((thread_t*) handle == nullptr || (thread_body_t) start_routine == nullptr || (void*) stack_space == nullptr) {
                    Riscv::w_user_reg(A0, -1);
                }
                else {
                    thread_t thread = new _thread((thread_body_t) start_routine, (void*) arg, (void*) stack_space);
                    thread_t* t_handle = (thread_t*) handle;
                    *t_handle = thread;
                    Riscv::w_user_reg(A0, 0);
                }
                break;
            }
            case THREAD_EXIT: {
                int ret = _thread::exit();
                Riscv::w_user_reg(A0, ret);
                break;
            }
            case THREAD_DISPATCH: {
                _thread::dispatch();
                break;
            }
            case SEM_OPEN: {
                uint64 handle = Riscv::r_user_reg(A1);
                uint64 init = Riscv::r_user_reg(A2);

                if ((sem_t*) handle == nullptr) {
                    Riscv::w_user_reg(A0, -1);
                }
                else {
                    sem_t sem = new _sem((int) init);
                    sem_t* s_handle = (sem_t*) handle;
                    *s_handle = sem;
                    Riscv::w_user_reg(A0, 0);
                }
                break;
            }
            case SEM_CLOSE: {
                uint64 handle = Riscv::r_user_reg(A1);

                if ((sem_t) handle == nullptr) {
                    Riscv::w_user_reg(A0, -1);
                }
                else {
                    sem_t s_handle = (sem_t) handle;
                    delete s_handle;
                    Riscv::w_user_reg(A0, 0);
                }
                break;
            }
            case SEM_WAIT: {
                uint64 handle = Riscv::r_user_reg(A1);

                if ((sem_t) handle == nullptr) {
                    Riscv::w_user_reg(A0, -1);
                }
                else {
                    sem_t s_handle = (sem_t) handle;
                    int ret = s_handle->wait();
                    Riscv::w_user_reg(A0, ret);
                }
                break;
            }
            case SEM_SIGNAL: {
                uint64 handle = Riscv::r_user_reg(A1);

                if ((sem_t) handle == nullptr) {
                    Riscv::w_user_reg(A0, -1);
                }
                else {
                    sem_t s_handle = (sem_t) handle;
                    int ret = s_handle->signal();
                    Riscv::w_user_reg(A0, ret);
                }
                break;
            }
            case TIME_SLEEP: {
                uint64 period = Riscv::r_user_reg(A1);
                Scheduler::Instance().putSleeping(_thread::running, period);
                _thread::dispatch();
                Riscv::w_user_reg(A0, 0);
                break;
            }*/
            case GETC: {
                //char c = _console::Instance()._getc();
                char c = __getc();
                __asm__ volatile("sd %0, 10*8(fp)" :: "r"(c)); //put return val in a0
                break;
            }
            case PUTC: {
                char c;
                __asm__ volatile("ld %0, 11*8(fp)" : "=r"(c)); //get the char from a1
                //_console::Instance()._putc(c);
                __putc(c);
                break;
            }
        }
    }
    /*else if (scause == TIMER_INTERRUPT) {
        mc_sip(SIP_SSIE);
        _thread::time++;
        Scheduler::Instance().updateSleeping();
        if (_thread::time >= DEFAULT_TIME_SLICE) {
            _thread::time = 0;
            _thread::dispatch();
        }
    }*/
    else if (scause == CONSOLE_INTERRUPT) {
        /*int irq = plic_claim();
        if (irq == 10) {
            _console::Instance().handler();
            //console_handler();
            plic_complete(irq);
        }*/
       console_handler();
    }
    else {
        //force quit
        /*printInteger(scause);
        uint32 val = 0x5555;
        uint64 addr = 0x100000;
        __asm__ volatile("sw %[val], 0(%[addr])" : : [val] "r" (val), [addr] "r" (addr));
        */
       uint64 sepc = r_sepc();
        printString("Unhandled scause: ");
        printInteger(scause);
        printString(", sepc: ");
        printInteger(sepc);
        while(1) {}
    }

    w_sepc(sepc);
    w_sstatus(sstatus);
}


/*void Riscv::handleSupervisorTrap()
{
    uint64 scause = r_scause();
    if (scause == 0x0000000000000008UL || scause == 0x0000000000000009UL)
    {
        // interrupt: no; cause code: environment call from U-mode(8) or S-mode(9)
        uint64 volatile sepc = r_sepc() + 4;
        uint64 volatile sstatus = r_sstatus();
        TCB::timeSliceCounter = 0;
        TCB::dispatch();
        w_sstatus(sstatus);
        w_sepc(sepc);
    }
    else if (scause == 0x8000000000000001UL)
    {
        // interrupt: yes; cause code: supervisor software interrupt (CLINT; machine timer interrupt)
        mc_sip(SIP_SSIP);
        TCB::timeSliceCounter++;
        if (TCB::timeSliceCounter >= TCB::running->getTimeSlice())
        {
            uint64 volatile sepc = r_sepc();
            uint64 volatile sstatus = r_sstatus();
            TCB::timeSliceCounter = 0;
            TCB::dispatch();
            w_sstatus(sstatus);
            w_sepc(sepc);
        }
    }
    else if (scause == 0x8000000000000009UL)
    {
        // interrupt: yes; cause code: supervisor external interrupt (PLIC; could be keyboard)
        console_handler();
    }
    else
    {
        // unexpected trap cause
    }
}*/
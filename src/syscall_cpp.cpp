#include "syscall_cpp.hpp"

Thread::Thread(void (*body)(void *), void *arg)
{
}

Thread::~Thread()
{
}

int Thread::start()
{
    return 0;
}

void Thread::dispatch()
{
}

int Thread::sleep(time_t)
{
    return 0;
}

Thread::Thread()
{
}

Semaphore::Semaphore(unsigned init)
{
}

Semaphore::~Semaphore()
{
}

int Semaphore::wait()
{
    return 0;
}

int Semaphore::signal()
{
    return 0;
}

void PeriodicThread::terminate()
{
}

PeriodicThread::PeriodicThread(time_t period)
{
}

char Console::getc()
{
    return ::getc();
}

void Console::putc(char c)
{
    ::putc(c);
}

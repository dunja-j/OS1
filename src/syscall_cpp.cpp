#include "../h/syscall_cpp.hpp"
#include "../h/syscall_c.hpp"
//#include "../lib/mem.h"

void* operator new (size_t n)
{
    return mem_alloc(n);
}

void* operator new[] (size_t n)
{
    return mem_alloc(n);
}

void operator delete(void *p) noexcept
{
    mem_free(p);
}

void operator delete[](void *p) noexcept
{
    mem_free(p);
}

Thread::Thread(thread_body_t body, void *arg)
{
    this->body = body;
    this->arg = arg;
}

Thread::Thread()
{
    this->body = runWrapper;
    this->arg = this;
}

Thread::~Thread() {}

int Thread::start()
{
    return thread_create(&myHandle, body, arg);
}

void Thread::runWrapper(void* arg) {
    ((Thread*)arg)->run();
}

void Thread::dispatch()
{
    thread_dispatch();
}

//------------------------------MODIFICATION--------------------------------------
int Thread::getThreadId()
{
    return ::getThreadId();
}
//-----------------------------------------------------------------------------------

int Thread::sleep(time_t)
{
    return 0;
}

Semaphore::Semaphore(unsigned init)
{
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore()
{
    sem_close(myHandle);
}

int Semaphore::wait()
{
    return sem_wait(myHandle);
}

int Semaphore::signal()
{
    return sem_signal(myHandle);
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

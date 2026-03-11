#include "syscall_cpp.hpp"

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

int Thread::sleep(time_t)
{
    return 0;
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

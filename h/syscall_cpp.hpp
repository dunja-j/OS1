#ifndef OS1_SYSCALL_CPP_HPP
#define OS1_SYSCALL_CPP_HPP
 
#include "../h/syscall_c.hpp"

void* operator new(size_t n);
void operator delete(void *p) noexcept;
 
class Thread { 
public: 
  Thread (thread_body_t body, void* arg); 
  virtual ~Thread (); 
 
  int start (); 
 
  static void dispatch ();
  //---------------------------- MODIFICATION--------------------------------------------
  static int getThreadId ();
  //-----------------------------------------------------------------------------------
  static int sleep (time_t); 
 
protected: 
  Thread (); 
  virtual void run () {} 
 
private: 
  thread_t myHandle; 
  thread_body_t body;
  void* arg;

  static void runWrapper(void* arg);
}; 
 
 
class Semaphore { 
public: 
 
  Semaphore (unsigned init = 1); 
  virtual ~Semaphore (); 
 
  int wait (); 
  int signal (); 
 
private: 
  sem_t myHandle; 
 
}; 
 
 
class PeriodicThread : public Thread { 
public: 
  void terminate (); 
 
protected: 
  PeriodicThread (time_t period); 
  virtual void periodicActivation () {} 
 
private: 
  time_t period; 
}; 
 
 
class Console { 
public: 
  static char getc (); 
  static void putc (char); 
}; 
#endif
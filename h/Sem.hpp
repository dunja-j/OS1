#ifndef OS1_SEM_HPP
#define OS1_SEM_HPP

#include "tcb.hpp"
#include "list.hpp"

class Sem {
public:
    typedef Sem* sem_t;

    int sem_signal();
    int sem_wait();
    int sem_close();

    static int sem_open(sem_t*, unsigned init);

private:
    int val;
    List<TCB> blocked;

    Sem (int val=1);
    ~Sem ();
};

#endif //OSPROJEKAT_SEMAPHORE_HPP

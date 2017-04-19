#include "um-common.h"

// FIXME: robust error handling
// FIXME: restart syscall when errno == EINTR

void umSyInitSpinlock(umSySpinlock *lock)
{
    int r = pthread_spin_init(&lock->lock, PTHREAD_PROCESS_SHARED);
    assert(r == 0);
}

void umSyDestroySpinlock(umSySpinlock *lock)
{
    int r = pthread_spin_destroy(&lock->lock);
    assert(r == 0);
}

void umSyAcquireSpinlock(umSySpinlock *lock)
{
    int r = pthread_spin_lock(&lock->lock);
    assert(r == 0);
}

void umSyReleaseSpinlock(umSySpinlock *lock)
{
    int r = pthread_spin_unlock(&lock->lock);
    assert(r == 0);
}


void umSyInitSemaphore(umSySemaphore *sem, unsigned int val)
{
    int r = sem_init(&sem->sem, 1, val);
    assert(r == 0);
}

void umSyDestroySemaphore(umSySemaphore *sem)
{
    int r = sem_destroy(&sem->sem);
    assert(r == 0);
}

void umSyWaitSemaphore(umSySemaphore *sem)
{
    int r = sem_wait(&sem->sem);
    assert(r == 0);
}

void umSySignalSemaphore(umSySemaphore *sem)
{
    int r = sem_post(&sem->sem);
    assert(r == 0);
}


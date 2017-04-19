#ifndef _ARCH_UM_SYNC_H
#define _ARCH_UM_SYNC_H


#define UM_SPINLOCK_SIZE 4
typedef struct {
    unsigned char lock[UM_SPINLOCK_SIZE];
} lock_t;


#ifdef COMPILING_LIBUM
typedef struct {
    pthread_spinlock_t lock;
} umSySpinlock;
static_assert(sizeof(umSySpinlock) <= sizeof(lock_t), "lock_t is too small");
#else
typedef lock_t umSySpinlock;
#endif


extern void umSyInitSpinlock(umSySpinlock *lock);
extern void umSyDestroySpinlock(umSySpinlock *lock);
extern void umSyAcquireSpinlock(umSySpinlock *lock);
extern void umSyReleaseSpinlock(umSySpinlock *lock);



#define UM_SEMAPHORE_SIZE 32
typedef struct {
    unsigned char sem[UM_SEMAPHORE_SIZE];
} semaphore_t;

#ifdef COMPILING_LIBUM
typedef struct {
    sem_t sem;
} umSySemaphore;
static_assert(sizeof(umSySemaphore) <= sizeof(semaphore_t), "semaphore_t is too small");
#else
typedef semaphore_t umSySemaphore;
#endif

extern void umSyInitSemaphore(umSySemaphore *sem, unsigned int val);
extern void umSyDestroySemaphore(umSySemaphore *sem);
extern void umSyWaitSemaphore(umSySemaphore *sem);
extern void umSySignalSemaphore(umSySemaphore *sem);


#endif

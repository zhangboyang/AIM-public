#ifndef _ARCH_SYNC_H
#define _ARCH_SYNC_H

#ifndef __ASSEMBLER__

typedef unsigned int lock_t;
#define EMPTY_LOCK(lock)	(UNLOCKED)

static inline
void spinlock_init(lock_t *lock)
{
}

static inline
void spin_lock(lock_t *lock)
{
}

static inline
void spin_unlock(lock_t *lock)
{
}

static inline
bool spin_is_locked(lock_t *lock)
{
	return true;
}

/* Semaphore */
typedef struct {
	int val;
	int limit;
} semaphore_t;

static inline
void semaphore_init(semaphore_t *sem, int val)
{
}

static inline
void semaphore_dec(semaphore_t *sem)
{
}

static inline
void semaphore_inc(semaphore_t *sem)
{
}

#endif /* __ASSEMBLER__ */

#endif /* _ARCH_SYNC_H */


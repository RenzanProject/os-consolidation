#include <kern/interrupt.h>
#include <kern/macrodef.h>
#include <kern/mutex.h>
#include <kern/percpu.h>

/* stub content for owner field */
#define STUB_THREAD ((void *)0xffef000000000000)

static thread_t *
get_current_thread(void)
{
        return percpu()->current_thread ? percpu()->current_thread
                                        : STUB_THREAD;
}

void
mutex_init(mutex_t *mutex)
{
        mutex->owner = NULL;
}

boolean
mutex_try_acquire(mutex_t *mutex)
{
        boolean success;
        void *  desired  = get_current_thread();
        void *  expected = NULL;
        success          = atomic_cmpxchg_strong_ptr(
            mutex->owner, expected, desired, __ATOMIC_SEQ_CST,
            __ATOMIC_ACQUIRE);

        return success;
}

#define MUTEX_ADAPTIVE_SPIN 100

void
mutex_acquire(mutex_t *mutex)
{
        boolean success = B_FALSE;
        while (!success) {
                for (uint i = 0; i < MUTEX_ADAPTIVE_SPIN; ++i) {
                        void *desired  = get_current_thread();
                        void *expected = NULL;
                        success        = atomic_cmpxchg_weak_ptr(
                            mutex->owner, expected, desired, __ATOMIC_SEQ_CST,
                            __ATOMIC_SEQ_CST);
                        if (success) break;

                        ASSERT(expected != get_current_thread());
                }

                sched_resched();
        }
}

void
mutex_release(mutex_t *mutex)
{
        void *owner = atomic_xchg_ptr(mutex->owner, NULL, __ATOMIC_SEQ_CST);

        VERIFY(
            owner == get_current_thread(),
            "invalid release: 0x%x trying to release mutex owned by 0x%x",
            get_current_thread(), owner);
}

void
mutex_acquire_dual(mutex_t *m1, mutex_t *m2)
{
        if ((uintptr)m1 < (uintptr)m2) {
                mutex_acquire(m1);
                mutex_acquire(m2);
        } else {
                mutex_acquire(m2);
                mutex_acquire(m1);
        }
}

void
mutex_release_dual(mutex_t *m1, mutex_t *m2)
{
        if ((uintptr)m1 < (uintptr)m2) {
                mutex_release(m2);
                mutex_release(m1);
        } else {
                mutex_release(m1);
                mutex_release(m2);
        }
}

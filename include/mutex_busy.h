#ifndef MUTEX_BUSY_
#define MUTEX_BUSY_

#include <stdatomic.h>
#include <stdbool.h>

typedef struct mutex_busy {
  atomic_llong* ticket;
  atomic_llong* turn;
} mutex_busy;

// Initializes the mutex
int mutex_busy_init(mutex_busy* mtxb);

// Locks the mutex, ensures only one thread accesses critical section
int mutex_busy_lock(mutex_busy* mtxb);

// Unlocks the mutex, allows others threads to access section
int mutex_busy_unlock(mutex_busy* mtxb);

// Destroys the mutex, frees up the memory
int mutex_busy_destroy(mutex_busy* mtxb);

#endif  // MUTEX_BUSY_
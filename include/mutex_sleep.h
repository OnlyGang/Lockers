#ifndef MUTEX_SLEEP_
#define MUTEX_SLEEP_

#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>

#include "thread_queue.h"

typedef struct mutex_atomic {
  atomic_bool lock;
  atomic_bool guard;
  thread_queue* q;
  pthread_t* locked_by;
} mutex_sleep;

void mutex_sleep_init(mutex_sleep* mtxq);

int mutex_sleep_lock(mutex_sleep* mtxq);

int mutex_sleep_unlock(mutex_sleep* mtxq);

int mutex_sleep_trylock(mutex_sleep* mtxq);

void mutex_sleep_destroy(mutex_sleep* mtxq);

#endif  // MUTEX_SLEEP_
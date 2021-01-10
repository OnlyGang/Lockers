#ifndef MUTEX_QUEUE_
#define MUTEX_QUEUE_

#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>

#include "thread_queue.h"

typedef struct mutex_queue {
  atomic_bool lock;
  atomic_bool guard;
  atomic_bool sleep_guard;
  thread_queue* q;
  pthread_t* locked_by;
} mutex_queue;

void signal_handler(int sig);

void mutex_queue_init(mutex_queue* mtxq);

int mutex_queue_lock(mutex_queue* mtxq);

int mutex_queue_unlock(mutex_queue* mtxq);

int mutex_queue_trylock(mutex_queue* mtxq);

void mutex_queue_destroy(mutex_queue* mtxq);

#endif  // MUTEX_QUEUE_
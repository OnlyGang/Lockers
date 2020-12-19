#include <pthread.h>
#include <stdbool.h>

#include "thread_queue.h"

typedef struct mutex_queue {
  bool locked;
  thread_queue* q;
  pthread_t* locked_by;
} mutex_queue;

void mutex_queue_init(mutex_queue* mtxq);

int mutex_queue_lock(mutex_queue* mtxq);

int mutex_queue_unlock(mutex_queue* mtxq);

int mutex_queue_trylock(mutex_queue* mtxq);

void mutex_queue_destroy(mutex_queue* mtxq);
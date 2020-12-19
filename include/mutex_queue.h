#include "thread_queue.h"

typedef struct mutex_queue {
  int lock;
  thread_queue* q;
} mutex_queue;

void mutex_queue_init(mutex_queue* mtxq);

int mutex_queue_lock(mutex_queue* mtxq);

int mutex_queue_unlock(mutex_queue* mtxq);

int mutex_queue_trylock(mutex_queue* mtxq);

void mutex_queue_remove(mutex_queue* mtxq);
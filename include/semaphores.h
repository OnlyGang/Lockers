#include <pthread.h>
#include <stdbool.h>

#include "thread_queue.h"

typedef struct semaphore {
  int S;
  pthread_mutex_t* mtx;  // This should be changed
  thread_queue* q;
  pthread_t* locked_by;
} semaphore;

void semaphore_init(semaphore* sem, int val);

int semaphore_wait(semaphore* sem);

int semaphore_post(semaphore* sem);

void semaphore_destroy(semaphore* sem);
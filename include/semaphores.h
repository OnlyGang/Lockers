#ifndef SEMAPHORES_
#define SEMAPHORES_

#include <pthread.h>
#include <stdbool.h>

#include "mutex_queue.h"

typedef struct semaphore {
  int S;
  mutex_queue* mtxq;
  thread_queue* sem_q;
} semaphore;

void signal_handler_2(int sig);

void semaphore_init(semaphore* sem, int val);

int semaphore_wait(semaphore* sem);

int semaphore_post(semaphore* sem);

void semaphore_destroy(semaphore* sem);

#endif  // SEMAPHORES_
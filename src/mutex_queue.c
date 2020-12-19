#include "../include/mutex_queue.h"

#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void mutex_queue_init(mutex_queue* mtxq) {
  mtxq->locked = false;
  mtxq->q = malloc(sizeof(thread_queue));
  mtxq->locked_by = NULL;
  queue_init(mtxq->q);
}

int mutex_queue_lock(mutex_queue* mtxq) {
  pthread_t* thr = malloc(sizeof(pthread_t));
  *thr = pthread_self();
  if (mtxq->locked == true) {
    push(mtxq->q, thr);
    printf("The thread %ld went to sleep\n", pthread_self());
    pthread_kill(*thr, SIGSTOP);
    // pause();  // Just in case, add if (pause() == -1), might better do
    // pthread_kill(sigstop)
  }

  mtxq->locked_by = thr;
  mtxq->locked = true;
  return 0;
}

int mutex_queue_unlock(mutex_queue* mtxq) {
  if (mtxq->locked == false) {
    perror("mutex_queue tried to unlock without being locked\n");
    return -1;
  }
  if (*(mtxq->locked_by) != pthread_self()) {
    perror("mutex_queue tried to be unlocked by another thread\n");
    return -1;
  }

  free(mtxq->locked_by);
  mtxq->locked_by = NULL;

  if (!empty(mtxq->q)) {
    pthread_t* next = pop(mtxq->q);
    if (pthread_kill(*next, SIGCONT) != 0) {
      perror("Couldn't wake up a thread\n");
      return errno;
    };
    free(next);
  } else {
    mtxq->locked = false;
  }

  return 0;
}

int mutex_queue_trylock(mutex_queue* mtxq) {
  return (mtxq->locked == false ? mutex_queue_lock(mtxq) : -1);
}

void mutex_queue_destroy(mutex_queue* mtxq) {
  if (mtxq->locked_by != NULL) {
    free(mtxq->locked_by);
  }
  queue_remove(mtxq->q);
}
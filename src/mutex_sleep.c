#define _XOPEN_SOURCE 500

#include "../include/mutex_sleep.h"

#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void mutex_sleep_init(mutex_sleep* mtxq) {
  atomic_store(&(mtxq->lock), 0);
  atomic_store(&(mtxq->guard), 0);
  mtxq->q = malloc(sizeof(thread_queue));
  mtxq->locked_by = NULL;
  queue_init(mtxq->q);
}

int mutex_sleep_lock(mutex_sleep* mtxq) {
  // printf("Thread %ld entered the lock\n", pthread_self() % 1000);

  // Test and set-ul
  while (__atomic_test_and_set(&(mtxq->guard), __ATOMIC_ACQ_REL) == 1)
    ;

  pthread_t* thr = malloc(sizeof(pthread_t));
  *thr = pthread_self();
  if (atomic_load(&(mtxq->lock))) {
    // printf("Thread %ld entered queue zone\n", pthread_self() % 1000);
    int hash_id = push(mtxq->q, thr);
    atomic_store(&(mtxq->guard), 0);
    while (mtxq->q->in_queue[hash_id]) {
      usleep((useconds_t)100);
    }
    // printf("Thread %ld assign the lock after sleep\n", pthread_self() %
    // 1000);
    mtxq->locked_by = thr;
  } else {
    atomic_store(&(mtxq->lock), 1);
    atomic_store(&(mtxq->guard), 0);
    mtxq->locked_by = thr;
  }
  // printf("Thread %ld passed the lock\n", pthread_self() % 1000);

  return 0;
}

int mutex_sleep_unlock(mutex_sleep* mtxq) {
  // TAS - Asteapta pana cand este bagat un thread in coada
  while (__atomic_test_and_set(&(mtxq->guard), __ATOMIC_ACQ_REL) == 1)
    ;

  // printf("Thread %ld entered the unlock\n", pthread_self() % 1000);

  if (atomic_load(&(mtxq->lock)) == false) {
    perror("Mutex tried to unlock without being locked\n");
    return -1;
  }
  if (*(mtxq->locked_by) != pthread_self()) {
    // printf("Me %ld Locked By %ld", pthread_self() % 1000,
    //        *(mtxq->locked_by) % 1000);
    perror("Mutex tried to be unlocked by another thread\n");
    return -1;
  }

  free(mtxq->locked_by);
  mtxq->locked_by = NULL;

  if (!empty(mtxq->q)) {
    pthread_t* next = pop(mtxq->q);
    // printf("Popped %ld\n", *next % 1000);
    (void)next;
  } else {
    atomic_store(&(mtxq->lock), false);
  }
  atomic_store(&(mtxq->guard), false);
  // printf("Thread %ld left unlock\n", pthread_self() % 1000);
  return 0;
}

int mutex_sleep_trylock(mutex_sleep* mtxq) {
  return (atomic_load(&(mtxq->lock)) == false ? mutex_sleep_lock(mtxq) : -1);
}

void mutex_sleep_destroy(mutex_sleep* mtxq) {
  if (mtxq->locked_by != NULL) {
    free(mtxq->locked_by);
  }
  queue_remove(mtxq->q);
}

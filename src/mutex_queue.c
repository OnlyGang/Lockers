#define _XOPEN_SOURCE 500

#include "../include/mutex_queue.h"

#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void signal_handler(int sig) {
  (void)sig;

  // printf("Caught signal %d\n", sig);
  signal(SIGUSR1, signal_handler);
}

void mutex_queue_init(mutex_queue* mtxq) {
  atomic_store(&(mtxq->lock), 0);
  atomic_store(&(mtxq->guard), 0);
  atomic_store(&(mtxq->sleep_guard), 0);
  mtxq->q = malloc(sizeof(thread_queue));
  mtxq->locked_by = NULL;
  queue_init(mtxq->q);
  signal(SIGUSR1, signal_handler);
}

int mutex_queue_lock(mutex_queue* mtxq) {
  // printf("Thread %ld entered the lock\n", pthread_self() % 1000);

  // Test and set-ul
  while (__atomic_test_and_set(&(mtxq->guard), __ATOMIC_ACQ_REL) == 1)
    ;

  pthread_t* thr = malloc(sizeof(pthread_t));
  *thr = pthread_self();
  if (atomic_load(&(mtxq->lock))) {
    /// PENTRU IMBUANATATIRE, SA ADAUG UN BUSY WAIT MIC

    // printf("Thread %ld entered queue zone\n", pthread_self() % 1000);
    int hash_id = push(mtxq->q, thr);
    atomic_store(&(mtxq->guard), 0);

    /* Sleep part */
    while (__atomic_test_and_set(&(mtxq->sleep_guard), __ATOMIC_ACQ_REL) == 1)
      ;
    if (mtxq->q->in_queue[hash_id]) {
      // printf("Thread %ld entered the sleep\n", pthread_self() % 1000);
      // printf("Going to sleep %d\n", hash_id);
      atomic_store(&(mtxq->sleep_guard), 0);
      pause();
      // printf("Wake up, %d\n", hash_id);
    }
    if (atomic_load(&(mtxq->sleep_guard)) == 1)
      atomic_store(&(mtxq->sleep_guard), 0);
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

int mutex_queue_unlock(mutex_queue* mtxq) {
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
    while (__atomic_test_and_set(&(mtxq->sleep_guard), __ATOMIC_ACQ_REL) == 1)
      ;
    usleep((useconds_t)200);
    pthread_t* next = pop(mtxq->q);
    if (pthread_kill(*next, SIGUSR1) != 0) {
      perror("Couldn't wake up the thread\n");
      return errno;
    };
    usleep((useconds_t)50);
    // printf("Popped %ld\n", *next % 1000);
    atomic_store(&(mtxq->sleep_guard), 0);
  } else {
    atomic_store(&(mtxq->lock), false);
  }
  atomic_store(&(mtxq->guard), false);
  // printf("Thread %ld left unlock\n", pthread_self() % 1000);
  return 0;
}

int mutex_queue_trylock(mutex_queue* mtxq) {
  return (atomic_load(&(mtxq->lock)) == false ? mutex_queue_lock(mtxq) : -1);
}

void mutex_queue_destroy(mutex_queue* mtxq) {
  if (mtxq->locked_by != NULL) {
    free(mtxq->locked_by);
  }
  queue_remove(mtxq->q);
}
